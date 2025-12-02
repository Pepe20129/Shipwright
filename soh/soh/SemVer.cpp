#include "SemVer.h"
#include "util.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <format>
#include <regex>
#include <sstream>

SemanticVersion::SemanticVersion(
    SemanticVersionInt major,
    SemanticVersionInt minor,
    SemanticVersionInt patch,
    std::string preRelease,
    std::string buildMetadata
) :
    major(major),
    minor(minor),
    patch(patch),
    preRelease(std::move(preRelease)),
    buildMetadata(std::move(buildMetadata)) {}

SemanticVersion SemanticVersion::New(
    SemanticVersionInt major,
    SemanticVersionInt minor,
    SemanticVersionInt patch,
    const std::string preRelease,
    const std::string buildMetadata
) {
    return SemanticVersion(major, minor, patch, preRelease, buildMetadata);
}

std::optional<SemanticVersion> SemanticVersion::Parse(const std::string& str) {
    std::string core;

    size_t plusPos = str.find('+');
    std::string buildMetadata;
    if (plusPos != std::string::npos) {
        buildMetadata = str.substr(plusPos + 1);
        core = str.substr(0, plusPos);
    } else {
        core = str;
    }

    size_t dashPos = str.find('-');
    std::string preRelease;
    if (dashPos != std::string::npos && dashPos < core.size()) {
        preRelease = core.substr(dashPos + 1);
        core = core.substr(0, dashPos);
    }

    std::stringstream ss(core);

    std::string majorStr;
    if (!std::getline(ss, majorStr, '.')) {
        return std::nullopt;
    }

    std::string minorStr;
    if (!std::getline(ss, minorStr, '.')) {
        return std::nullopt;
    }

    std::string patchStr;
    if (!std::getline(ss, patchStr, '.')) {
        return std::nullopt;
    }

    // can't start with zeros unless the value is "0"
    if (
        (majorStr.size() > 1 && majorStr[0] == '0') ||
        (minorStr.size() > 1 && minorStr[0] == '0') ||
        (patchStr.size() > 1 && patchStr[0] == '0')
    ) {
        return std::nullopt;
    }

    // check if they're numbers
    if (!std::all_of(majorStr.begin(), majorStr.end(), ::isdigit)) {
        return std::nullopt;
    }

    if (!std::all_of(minorStr.begin(), minorStr.end(), ::isdigit)) {
        return std::nullopt;
    }

    if (!std::all_of(patchStr.begin(), patchStr.end(), ::isdigit)) {
        return std::nullopt;
    }

    try {
        long long major = std::stoll(majorStr);
        long long minor = std::stoll(minorStr);
        long long patch = std::stoll(patchStr);

        // check if they're in range
        if (
            major < 0 || major > std::numeric_limits<SemanticVersionInt>::max() ||
            minor < 0 || minor > std::numeric_limits<SemanticVersionInt>::max() ||
            patch < 0 || patch > std::numeric_limits<SemanticVersionInt>::max()
        ) {
            return std::nullopt;
        }

        return SemanticVersion::New(
            static_cast<SemanticVersionInt>(major),
            static_cast<SemanticVersionInt>(minor),
            static_cast<SemanticVersionInt>(patch),
            preRelease,
            buildMetadata
        );
    } catch (std::invalid_argument const& ex) {
        return std::nullopt;
    } catch (std::out_of_range const& ex) {
        return std::nullopt;
    }
}

SemanticVersionInt SemanticVersion::GetMajor() const {
    return major;
}

SemanticVersionInt SemanticVersion::GetMinor() const {
    return minor;
}

SemanticVersionInt SemanticVersion::GetPatch() const {
    return patch;
}

std::string SemanticVersion::GetPreRelease() const {
    return preRelease;
}

std::string SemanticVersion::GetBuildMetadata() const {
    return buildMetadata;
}

std::string SemanticVersion::ToString() const {
    std::string str = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);

    if (preRelease != "") {
        str += "-" + preRelease;
    }

    if (buildMetadata != "") {
        str += "+" + buildMetadata;
    }

    return str;
}

static bool StringIsNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

static int ComparePreReleases(const std::string& a, const std::string& b) {
    if (a.empty() && b.empty()) return 0;
    if (a.empty()) return 1;
    if (b.empty()) return -1;

    std::vector<std::string> aParts = SohUtils::SplitString(a, '.');
    std::vector<std::string> bParts = SohUtils::SplitString(b, '.');

    size_t len = std::min(aParts.size(), bParts.size());

    for (size_t i = 0; i < len; ++i) {
        const std::string& x = aParts[i];
        const std::string& y = bParts[i];

        bool xIsNum = StringIsNumber(x);
        bool yIsNum = StringIsNumber(y);

        if (xIsNum && yIsNum) {
            int xVal = std::stoi(x);
            int yVal = std::stoi(y);

            if (xVal != yVal) {
                return xVal < yVal ? -1 : 1;
            }
        } else if (xIsNum) {
            return -1; // numeric < alpha
        } else if (yIsNum) {
            return 1;
        } else if (x != y) {
            return x < y ? -1 : 1;
        }
    }

    if (aParts.size() != bParts.size()) {
        return aParts.size() < bParts.size() ? -1 : 1;
    }

    return 0;
}

int SemanticVersion::Compare(const SemanticVersion& a, const SemanticVersion& b) {
    if (a.major != b.major) {
        return a.major < b.major ? -1 : 1;
    }

    if (a.minor != b.minor) {
        return a.minor < b.minor ? -1 : 1;
    }

    if (a.patch != b.patch) {
        return a.patch < b.patch ? -1 : 1;
    }

    return ComparePreReleases(a.preRelease, b.preRelease);
}

SemanticVersion SemanticVersion::Clone() const {
    return SemanticVersion::New(
        major,
        minor,
        patch,
        preRelease,
        buildMetadata
    );
}

std::optional<SemanticVersion> SemanticVersion::Next() const {
    // No pre-release: bump patch, start pre-release as "0"
    if (preRelease.empty()) {
        SemanticVersionInt newMajor = major;
        SemanticVersionInt newMinor = minor;
        SemanticVersionInt newPatch = patch;

        if (newPatch != std::numeric_limits<SemanticVersionInt>::max()) {
            newPatch += 1;
        } else {
            newPatch = 0;
            if (newMinor != std::numeric_limits<SemanticVersionInt>::max()) {
                newMinor += 1;
            } else {
                newMinor = 0;
                if (newMajor != std::numeric_limits<SemanticVersionInt>::max()) {
                    newMajor += 1;
                } else {
                    return std::nullopt;
                }
            }
        }

        return SemanticVersion::New(
            newMajor,
            newMinor,
            newPatch,
            "0",
            ""
        );
    } else {
        // Pre-release exists: append ".0"
        std::string newPreRelease = preRelease + ".0";
        return SemanticVersion::New(
            major,
            minor,
            patch,
            newPreRelease,
            ""
        );
    }
}

bool SemanticVersion::operator==(const SemanticVersion& other) const {
    return Compare(*this, other) == 0;
}

bool SemanticVersion::operator!=(const SemanticVersion& other) const {
    return Compare(*this, other) != 0;
}

bool SemanticVersion::operator<(const SemanticVersion& other) const {
    return Compare(*this, other) < 0;
}

bool SemanticVersion::operator<=(const SemanticVersion& other) const {
    return Compare(*this, other) <= 0;
}

bool SemanticVersion::operator>(const SemanticVersion& other) const {
    return Compare(*this, other) > 0;
}

bool SemanticVersion::operator>=(const SemanticVersion& other) const {
    return Compare(*this, other) >= 0;
}



static bool HaveSameTuple(SemanticVersion semanticVersion1, SemanticVersion semanticVersion2) {
    return semanticVersion1.GetMajor() == semanticVersion2.GetMajor() &&
        semanticVersion1.GetMinor() == semanticVersion2.GetMinor() &&
        semanticVersion1.GetPatch() == semanticVersion2.GetPatch();
}

static const std::regex PARTIAL_REGEX(
    R"(^(?<major>0|[1-9]\d*)(?:\.(?<minor>0|[1-9]\d*)(?:\.(?<patch>0|[1-9]\d*)(?:-(?<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?)?)?$)",
    std::regex_constants::ECMAScript
);

static const std::regex X_REGEX(
    R"(^(?<major>0|[1-9]\d*)\.(?:(?<minor>[xX*])(?:\.(?<patch>[xX*]))?|(?<minor>0|[1-9]\d*)(?:\.(?<patch>[xX*])))(?:-(?<prerelease>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?<buildmetadata>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)",
    std::regex_constants::ECMAScript
);

static const std::regex PARTIAL_HYPHEN_REGEX(
    R"(^(?<major1>0|[1-9]\d*)(?:\.(?<minor1>0|[1-9]\d*)(?:\.(?<patch1>0|[1-9]\d*)(?:-(?<prerelease1>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?<buildmetadata1>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?)?)? - (?<major2>0|[1-9]\d*)(?:\.(?<minor2>0|[1-9]\d*)(?:\.(?<patch2>0|[1-9]\d*)(?:-(?<prerelease2>(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+(?<buildmetadata2>[0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?)?)?$)",
    std::regex_constants::ECMAScript
);

static std::optional<SemanticVersion> SemanticVersionFromPartialMatch(const std::smatch& match) {
    if (match.empty()) {
        return std::nullopt;
    }

    // Named capture groups are not supported in C++ 20
    // Capture group order (start on 1 for some reason?):
    // [1] major
    // [2] minor
    // [3] patch
    // [4] prerelease
    // [5] buildmetadata

    std::string major = match[1].matched ? match[1].str() : "0";
    std::string minor = (match.size() > 2 && match[2].matched) ? match[2].str() : "0";
    std::string patch = (match.size() > 3 && match[3].matched) ? match[3].str() : "0";

    std::string result = major + "." + minor + "." + patch;

    if (match.size() > 4 && match[4].matched && !match[4].str().empty()) {
        result += "-" + match[4].str();
    }

    if (match.size() > 5 && match[5].matched && !match[5].str().empty()) {
        result += "+" + match[5].str();
    }

    return SemanticVersion::Parse(result);
}

static std::optional<SemanticVersion> SemanticVersionFromXMatch(const std::smatch& match) {
    if (match.empty()) {
        return std::nullopt;
    }

    // Named capture groups are not supported in C++ 20
    // Capture group order (start on 1 for some reason?):
    // [1] major
    // [2] minor
    // [3] patch
    // [4] prerelease
    // [5] buildmetadata

    std::string major = match[1].matched ? match[1].str() : "0";

    std::string minor = "0";
    if (match.size() > 2 && match[2].matched) {
        std::string minorValue = match[2].str();
        if (minorValue != "x" && minorValue != "X" && minorValue != "*") {
            minor = minorValue;
        }
    }

    std::string str = major + "." + minor + "." + "0";

    if (match.size() > 4 && match[4].matched && !match[4].str().empty()) {
        str += "-" + match[4].str();
    }

    if (match.size() > 5 && match[5].matched && !match[5].str().empty()) {
        str += "+" + match[5].str();
    }

    return SemanticVersion::Parse(str);
}

std::optional<SemanticVersionComparator> SemanticVersionComparator::Parse(const std::string& str, bool includePrereleases) {
    if (str == "" || str == "*" || str == "x" || str == "X") {
        return SemanticVersionComparator::New(
            std::nullopt,
            std::nullopt,
            includePrereleases
        );
    }

    if (str.starts_with("=")) {
        std::smatch match;

        std::string sub = str.substr(1);
        // The entire substring matched
        if (std::regex_match(sub, match, PARTIAL_REGEX)) {
            std::optional<SemanticVersion> semanticVersion = SemanticVersionFromPartialMatch(match);

            if (!semanticVersion.has_value()) {
                return std::nullopt;
            }

            return SemanticVersionComparator::New(
                semanticVersion.value(),
                semanticVersion.value().Next(),
                includePrereleases
            );
        }
    } else if (str.starts_with(">=")) {
        std::smatch match;

        std::string sub = str.substr(2);
        // The entire substring matched
        if (std::regex_match(sub, match, PARTIAL_REGEX)) {
            std::optional<SemanticVersion> min = SemanticVersionFromPartialMatch(match);

            if (!min.has_value()) {
                return std::nullopt;
            }

            return SemanticVersionComparator::New(
                min.value(),
                std::nullopt,
                includePrereleases
            );
        }
    } else if (str.starts_with(">")) {
        std::smatch match;

        std::string sub = str.substr(1);
        // The entire substring matched
        if (std::regex_match(sub, match, PARTIAL_REGEX)) {
            std::optional<SemanticVersion> min = SemanticVersionFromPartialMatch(match);

            if (!min.has_value()) {
                return std::nullopt;
            }

            return SemanticVersionComparator::New(
                min.value().Next(),
                std::nullopt,
                includePrereleases
            );
        }
    } else if (str.starts_with("<=")) {
        std::smatch match;

        std::string sub = str.substr(2);
        // The entire substring matched
        if (std::regex_match(sub, match, PARTIAL_REGEX)) {
            std::optional<SemanticVersion> max = SemanticVersionFromPartialMatch(match);

            if (!max.has_value()) {
                return std::nullopt;
            }

            return SemanticVersionComparator::New(
                std::nullopt,
                max.value().Next(),
                includePrereleases
            );
        }
    } else if (str.starts_with("<")) {
        std::smatch match;

        std::string sub = str.substr(1);
        // The entire substring matched
        if (std::regex_match(sub, match, PARTIAL_REGEX)) {
            std::optional<SemanticVersion> max = SemanticVersionFromPartialMatch(match);

            if (!max.has_value()) {
                return std::nullopt;
            }

            return SemanticVersionComparator::New(
                std::nullopt,
                max.value(),
                includePrereleases
            );
        }
    } else if (str.starts_with("~")) {
        std::smatch match;

        std::string sub = str.substr(1);
        // The entire substring matched
        if (std::regex_match(sub, match, PARTIAL_REGEX)) {
            std::optional<SemanticVersion> semanticVersion = SemanticVersionFromPartialMatch(match);

            if (!semanticVersion.has_value()) {
                return std::nullopt;
            }

            // TODO: take std::numeric_limits<SemanticVersionInt>::max() into account
            if (match.size() > 2 && match[2].matched) {
                return SemanticVersionComparator::New(
                    semanticVersion.value(),
                    SemanticVersion::New(
                        semanticVersion.value().GetMajor(),
                        semanticVersion.value().GetMinor() + 1,
                        0,
                        "0"
                    ),
                    includePrereleases
                );
            } else {
                return SemanticVersionComparator::New(
                    semanticVersion.value(),
                    SemanticVersion::New(
                        semanticVersion.value().GetMajor() + 1,
                        0,
                        0,
                        "0"
                    ),
                    includePrereleases
                );
            }
        }
    } else if (str.starts_with("^")) {
        std::smatch match;

        std::string sub = str.substr(1);
        // The entire substring matched PARTIAL_REGEX
        if (std::regex_match(sub, match, PARTIAL_REGEX)) {
            std::optional<SemanticVersion> min = SemanticVersionFromPartialMatch(match);

            if (!min.has_value()) {
                return std::nullopt;
            }

            std::optional<SemanticVersion> max = std::nullopt;

            // TODO: take std::numeric_limits<SemanticVersionInt>::max() into account
            if (!(match.size() > 3 && match[3].matched)) {
                if (match.size() > 2 && match[2].matched) {
                    if (min.value().GetMajor() != 0) {
                        max = SemanticVersion::New(
                            min.value().GetMajor() + 1,
                            0,
                            0,
                            "0"
                        );
                    } else {
                        max = SemanticVersion::New(
                            min.value().GetMajor(),
                            min.value().GetMinor() + 1,
                            0,
                            "0"
                        );
                    }
                } else {
                    max = SemanticVersion::New(
                        min.value().GetMajor() + 1,
                        0,
                        0,
                        "0"
                    );
                }
            } else {
                if (min.value().GetMajor() == 0) {
                    max = SemanticVersion::New(
                        min.value().GetMajor(),
                        min.value().GetMinor() + 1,
                        0,
                        "0"
                    );
                } else {
                    max = SemanticVersion::New(
                        min.value().GetMajor() + 1,
                        0,
                        0,
                        "0"
                    );
                }
            }

            return SemanticVersionComparator::New(
                min.value(),
                max,
                includePrereleases
            );
        }

        // The entire substring matched X_REGEX
        if (std::regex_match(sub, match, X_REGEX)) {
            std::optional<SemanticVersion> min = SemanticVersionFromXMatch(match);

            if (!min.has_value()) {
                return std::nullopt;
            }

            std::optional<SemanticVersion> max = std::nullopt;

            // TODO: take std::numeric_limits<SemanticVersionInt>::max() into account
            if (
                !(match.size() > 3 && match[3].matched) ||
                match[3].str() == "x" ||
                match[3].str() == "X" ||
                match[3].str() == "*"
            ) {
                if (
                    !(match.size() > 2 && match[2].matched) ||
                    match[2].str() == "x" ||
                    match[2].str() == "X" ||
                    match[2].str() == "*"
                ) {
                    if (min.value().GetMajor() != 0) {
                        max = SemanticVersion::New(
                            min.value().GetMajor() + 1,
                            0,
                            0,
                            "0"
                        );
                    } else {
                        max = SemanticVersion::New(
                            min.value().GetMajor(),
                            min.value().GetMinor() + 1,
                            0,
                            "0"
                        );
                    }
                } else {
                    max = SemanticVersion::New(
                        min.value().GetMajor() + 1,
                        0,
                        0,
                        "0"
                    );
                }
            }

            return SemanticVersionComparator::New(
                min.value(),
                max,
                includePrereleases
            );
        }
    } else {
        std::smatch match;

        // The entire string matched
        if (std::regex_match(str, match, PARTIAL_REGEX)) {
            std::optional<SemanticVersion> min = SemanticVersionFromPartialMatch(match);

            if (!min.has_value()) {
                return std::nullopt;
            }

            return SemanticVersionComparator::New(
                min.value(),
                min.value().Next(),
                includePrereleases
            );
        }

        // The entire string matched
        if (std::regex_match(str, match, X_REGEX)) {
            std::optional<SemanticVersion> min = SemanticVersionFromXMatch(match);

            if (!min.has_value()) {
                return std::nullopt;
            }

            bool m = match.size() > 2 && match[2].matched && (match[2].str() == "x" || match[2].str() == "X" || match[2].str() == "*");
            bool p = match.size() > 3 && match[3].matched && (match[3].str() == "x" || match[3].str() == "X" || match[3].str() == "*");

            std::optional<SemanticVersion> max = std::nullopt;

            if (m) {
                max = SemanticVersion::New(
                    min.value().GetMajor() + 1,
                    0,
                    0,
                    "0"
                );
            } else if (p) {
                max = SemanticVersion::New(
                    min.value().GetMajor(),
                    min.value().GetMinor() + 1,
                    0,
                    "0"
                );
            } else {
                return std::nullopt;
            }

            return SemanticVersionComparator::New(
                min.value(),
                max,
                includePrereleases
            );
        }

        // The entire string matched
        if (std::regex_match(str, match, PARTIAL_HYPHEN_REGEX)) {
            // Named capture groups are not supported in C++ 20
            // Capture group order (start on 1 for some reason?):
            // [1]  major1
            // [2]  minor1
            // [3]  patch1
            // [4]  prerelease1
            // [5]  buildmetadata1
            // [6]  major2
            // [7]  minor2
            // [8]  patch2
            // [9]  prerelease2
            // [10] buildmetadata2

            if (
                match.size() <= 9 ||
                !(
                    match[1].matched &&
                    match[2].matched &&
                    match[3].matched &&
                    match[6].matched &&
                    match[7].matched &&
                    match[8].matched
                )
            ) {
                return std::nullopt;
            }

            try {
                long long major1 = std::stoll(match[1].str());
                long long minor1 = std::stoll(match[2].str());
                long long patch1 = std::stoll(match[3].str());

                long long major2 = std::stoll(match[6].str());
                long long minor2 = std::stoll(match[7].str());
                long long patch2 = std::stoll(match[8].str());

                SemanticVersion min = SemanticVersion::New(
                    static_cast<SemanticVersionInt>(major1),
                    static_cast<SemanticVersionInt>(minor1),
                    static_cast<SemanticVersionInt>(patch1),
                    (match[4].matched ? match[4].str() : "")
                );

                SemanticVersion max = SemanticVersion::New(
                    static_cast<SemanticVersionInt>(major2),
                    static_cast<SemanticVersionInt>(minor2),
                    static_cast<SemanticVersionInt>(patch2),
                    (match[9].matched ? match[9].str() : "")
                );

                return SemanticVersionComparator::New(
                    min,
                    max.Next(),
                    includePrereleases
                );
            } catch (std::invalid_argument const& ex) {
                return std::nullopt;
            } catch (std::out_of_range const& ex) {
                return std::nullopt;
            }
        }
    }

    return std::nullopt;
}

SemanticVersionComparator::SemanticVersionComparator(
    std::optional<SemanticVersion> min,
    std::optional<SemanticVersion> max,
    bool includePrereleases
) :
    min(min),
    max(max),
    includePrereleases(includePrereleases) {}

SemanticVersionComparator SemanticVersionComparator::New(
    std::optional<SemanticVersion> min,
    std::optional<SemanticVersion> max,
    bool includePrereleases
) {
    return SemanticVersionComparator(min, max, includePrereleases);
}

bool SemanticVersionComparator::Includes(const SemanticVersion& semanticVersion) const {
    if ((min.has_value() && semanticVersion < min.value()) || (max.has_value() && semanticVersion >= max.value())) {
        return false;
    }
    //if we don't include pre-releases of a different [major, minor, patch] tuple and the version specified has a pre-release
    if (!includePrereleases && semanticVersion.GetPreRelease() != "") {
        //if (the version specified has (a different tuple from min) and (a different tuple from max)) or
        //(it has the same tuple as one of them and that one doesn't have a pre-release), return false
        //                           ^_______________|
        if (((!min.has_value() || !HaveSameTuple(semanticVersion, min.value())) && (!max.has_value() || !HaveSameTuple(semanticVersion, max.value()))) ||
            (min.has_value() && HaveSameTuple(semanticVersion, min.value()) && (min.value().GetPreRelease() == "" || min.value().GetPreRelease() == "0")) ||
            (max.has_value() && HaveSameTuple(semanticVersion, max.value()) && (max.value().GetPreRelease() == "" || max.value().GetPreRelease() == "0"))) {
            return false;
        } else {
            //this only happens if the version specified has a pre-release and the same [major, minor, patch] tuple as one of the limit ones which also has a pre-release
            return true;
        }
    }
    return true;
}

std::string SemanticVersionComparator::ToString() const {
    if (min.has_value()) {
        if (max.has_value()) {
            if (max.value().GetPreRelease() != "0" || max.value().GetPatch() == 0) {
                // TODO
                return "";
            }

            SemanticVersion semVer = SemanticVersion::New(
                max.value().GetMajor(),
                max.value().GetMinor(),
                max.value().GetPatch() - 1
            );

            return min.value().ToString() + " - " + semVer.ToString();
        } else {
            return ">=" + min.value().ToString();
        }
    } else {
        if (max.has_value()) {
            return "<" + max.value().ToString();
        } else {
            return "*";
        }
    }
}

SemanticVersionComparator SemanticVersionComparator::Clone() const {
    return SemanticVersionComparator::New(
        min.has_value() ? std::optional(min.value().Clone()) : std::nullopt,
        max.has_value() ? std::optional(max.value().Clone()) : std::nullopt,
        includePrereleases
    );
}

bool SemanticVersionComparator::operator==(const SemanticVersionComparator& other) const {
    return min == other.min && max == other.max && includePrereleases == other.includePrereleases;
}

bool SemanticVersionComparator::operator!=(const SemanticVersionComparator& other) const {
    return !(*this == other);
}



SemanticVersionRange::SemanticVersionRange(
    std::vector<std::vector<SemanticVersionComparator>> comparatorSets
) : comparatorSets(comparatorSets) {}

SemanticVersionRange SemanticVersionRange::New(
    std::vector<std::vector<SemanticVersionComparator>> comparatorSets
) {
    return SemanticVersionRange(comparatorSets);
}

std::optional<SemanticVersionRange> SemanticVersionRange::Parse(const std::string& str, bool includePrereleases) {
    std::vector<std::vector<SemanticVersionComparator>> comparatorSets;

    std::vector<std::string> comparatorSetStrings = SohUtils::SplitString(str, " || ");

    for (const std::string& comparatorSetString : comparatorSetStrings) {
        std::vector<std::string> comparatorStrings = SohUtils::SplitString(comparatorSetString, " ");

        std::vector<SemanticVersionComparator> comparators;

        for (const std::string& comparatorString : comparatorStrings) {
            SemanticVersionComparator comparator = SemanticVersionComparator::Parse(comparatorString, includePrereleases);

            comparators.push_back(comparator);
        }

        comparatorStrings.push_back(comparatorSets);
    }

    return SemanticVersionRange::New(comparatorSets);
}

bool SemanticVersionRange::Includes(const SemanticVersion& semanticVersion) const {
    for (const std::vector<SemanticVersionComparator>& comparatorSet : comparatorSets) {
        bool all = true;
        for (const SemanticVersionComparator& comparator : comparatorSet) {
            if (!comparator.Includes(semanticVersion)) {
                all = false;
                break;
            }
        }

        if (all) {
            return true;
        }
    }

    return false;
}

std::string SemanticVersionRange::ToString() const {
    if (comparatorSets.size() == 0) {
        assert(false);
        return "";
    }

    std::string str = "";
    for (const std::vector<SemanticVersionComparator>& comparatorSet : comparatorSets) {
        if (comparatorSet.size() == 0) {
            assert(false);
            return "";
        }

        for (const SemanticVersionComparator& comparator : comparatorSet) {
            str += comparator.ToString() + " ";
        }
        str += "|| ";
    }

    str.pop_back();
    str.pop_back();
    str.pop_back();
    str.pop_back();

    return str;
}

SemanticVersionRange SemanticVersionRange::Clone() const {
    // TODO: Actually deep copy
    return SemanticVersionRange::New(
        comparatorSets
    );
}

bool SemanticVersionRange::operator==(const SemanticVersionRange& other) const {
    if (comparatorSets.size() == 0 && other.comparatorSets.size() == 0) {
        return true;
    }

    if (comparatorSets.size() != other.comparatorSets.size()) {
        return false;
    }

    for (int i = 0; i < comparatorSets.size(); i += 1) {
        if (comparatorSets[i].size() != other.comparatorSets[i].size()) {
            return false;
        }

        for (int j = 0; j < comparatorSets[i].size(); j += 1) {
            if (comparatorSets[i][j] != other.comparatorSets[i][j]) {
                return false;
            }
        }
    }

	return true;
}

bool SemanticVersionRange::operator!=(const SemanticVersionRange& other) const {
    return !(*this == other);
}
