#pragma once

#include "libultraship/libultra/types.h"

#include <optional>
#include <string>
#include <vector>

// SemVer does not say what the upper limit for the integers is, we have to choose (must be smaller or equal to "long long" as it's used in parsing)
typedef u8 SemanticVersionInt;

/**
 * @brief A Semantic Version as defined by SemVer 2.0.0 (https://semver.org/)
 *
 * Immutable
 */
typedef struct SemanticVersion final {
    public:
        /**
         * @brief Parses a SemanticVersion from a string
         */
        static std::optional<SemanticVersion> Parse(const std::string& str);

        /**
         * @brief Creates a new SemanticVersion from the provided arguments
         */
        static SemanticVersion New(
            SemanticVersionInt major,
            SemanticVersionInt minor,
            SemanticVersionInt patch,
            std::string preRelease = "",
            std::string buildMetadata = ""
        );

        SemanticVersionInt GetMajor() const;
        SemanticVersionInt GetMinor() const;
        SemanticVersionInt GetPatch() const;
        std::string GetPreRelease() const;
        std::string GetBuildMetadata() const;

        std::string ToString() const;

        /**
         * @brief Compares 2 SemanticVersions and returns -1, 0 or 1 depending on the result of the comparison
         */
        static int Compare(const SemanticVersion& a, const SemanticVersion& b);

        /**
         * @brief Creates a deep copy of this SemanticVersion
         */
        SemanticVersion Clone() const;

        /**
         * @brief Gets the immediately following SemanticVersion of this SemanticVersion
         *
         * There will be no possible SemanticVersions that compare between this SemanticVersion and the output SemanticVersion
         *
         * Returns std::nullopt if this SemanticVersion is maximum representable one
         */
        std::optional<SemanticVersion> Next() const;

        bool operator==(const SemanticVersion& other) const;
        bool operator!=(const SemanticVersion& other) const;
        bool operator<(const SemanticVersion& other) const;
        bool operator<=(const SemanticVersion& other) const;
        bool operator>(const SemanticVersion& other) const;
        bool operator>=(const SemanticVersion& other) const;

    private:
        SemanticVersion(
            SemanticVersionInt major,
            SemanticVersionInt minor,
            SemanticVersionInt patch,
            std::string preRelease,
            std::string buildMetadata
        );

        SemanticVersion() = delete;

        SemanticVersionInt major;
        SemanticVersionInt minor;
        SemanticVersionInt patch;
        std::string preRelease;
        std::string buildMetadata;
} SemanticVersion;

/**
 * @brief A Semantic Version Comparator as defined by NPM Semver ranges (https://github.com/npm/node-semver/blob/main/README.md#ranges)
 *
 * Immutable
 */
typedef struct SemanticVersionComparator final {
    public:
        /**
         * @brief Parses a SemanticVersionComparator from a string
         */
        static std::optional<SemanticVersionComparator> Parse(const std::string& str, bool includePrereleases);

        /**
         * @brief Creates a new SemanticVersionComparator from the provided arguments
         */
        static SemanticVersionComparator New(
            std::optional<SemanticVersion> min,
            std::optional<SemanticVersion> max,
            bool includePrereleases
        );

        bool Includes(const SemanticVersion& semanticVersion) const;

	    std::string ToString() const;

        /**
         * @brief Creates a deep copy of this SemanticVersionComparator
         */
        SemanticVersionComparator Clone() const;

        bool operator==(const SemanticVersionComparator& other) const;
        bool operator!=(const SemanticVersionComparator& other) const;

    private:
        SemanticVersionComparator(
            std::optional<SemanticVersion> min,
            std::optional<SemanticVersion> max,
            bool includePrereleases
        );

        SemanticVersionComparator() = delete;

        /**
         * @brief The minimum version for this comparator (inclusive)
         */
        std::optional<SemanticVersion> min;

        /**
         * @brief The maximum version for this comparator (exclusive)
         */
        std::optional<SemanticVersion> max;

        /**
         * @brief Whether pre-releases of a different [major, minor, patch] tuple should be included in the range or not
         */
        bool includePrereleases;
} SemanticVersionComparator;

/**
 * @brief A Semantic Version Range as defined by NPM Semver ranges (https://github.com/npm/node-semver/blob/main/README.md#ranges)
 *
 * Immutable
 */
typedef struct SemanticVersionRange final {
    public:
        /**
         * @brief Parses a SemanticVersionRange from a string
         */
        static std::optional<SemanticVersionRange> Parse(const std::string& str, bool includePrereleases);

        /**
         * @brief Creates a new SemanticVersionRange from the provided arguments
         */
        static SemanticVersionRange New(
            std::vector<std::vector<SemanticVersionComparator>> comparatorSets
        );

        bool Includes(const SemanticVersion& semanticVersion) const;

	    std::string ToString() const;

        /**
         * @brief Creates a deep copy of this SemanticVersionRange
         */
        SemanticVersionRange Clone() const;

        bool operator==(const SemanticVersionRange& other) const;
        bool operator!=(const SemanticVersionRange& other) const;

    private:
        SemanticVersionRange(
            std::vector<std::vector<SemanticVersionComparator>> comparatorSets
        );

        SemanticVersionRange() = delete;

        std::vector<std::vector<SemanticVersionComparator>> comparatorSets;
} SemanticVersionRange;
