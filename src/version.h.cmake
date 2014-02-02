/*
 * Copyright (C) 2014 David Faure <faure@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _TMDBQT_VERSION_H_
#define _TMDBQT_VERSION_H_

#include "tmdbqt_export.h"

/// @brief TmdbQt version as string at compile time.
#define LIBTMDBQT_VERSION_STRING "${CMAKE_LIBTMDBQT_VERSION_STRING}"

/// @brief The major TmdbQt version number at compile time
#define LIBTMDBQT_VERSION_MAJOR ${CMAKE_LIBTMDBQT_VERSION_MAJOR}

/// @brief The minor TmdbQt version number at compile time
#define LIBTMDBQT_VERSION_MINOR ${CMAKE_LIBTMDBQT_VERSION_MINOR}

/// @brief The TmdbQt release version number at compile time
#define LIBTMDBQT_VERSION_RELEASE ${CMAKE_LIBTMDBQT_VERSION_PATCH}

/**
 * \brief Create a unique number from the major, minor and release number of a %TmdbQt version
 *
 * This function can be used for preprocessing. For version information at runtime
 * use the version methods in the TmdbQt namespace.
 */
#define LIBTMDBQT_MAKE_VERSION( a,b,c ) (((a) << 16) | ((b) << 8) | (c))

/**
 * \brief %TmdbQt Version as a unique number at compile time
 *
 * This macro calculates the %TmdbQt version into a number. It is mainly used
 * through LIBTMDBQT_IS_VERSION in preprocessing. For version information at runtime
 * use the version methods in the TmdbQt namespace.
 */
#define LIBTMDBQT_VERSION \
    LIBTMDBQT_MAKE_VERSION(LIBTMDBQT_VERSION_MAJOR,LIBTMDBQT_VERSION_MINOR,LIBTMDBQT_VERSION_RELEASE)

/**
 * \brief Check if the %TmdbQt version matches a certain version or is higher
 *
 * This macro is typically used to compile conditionally a part of code:
 * \code
 * #if LIBTMDBQT_IS_VERSION(2,1)
 * // Code for TmdbQt 2.1
 * #else
 * // Code for TmdbQt 2.0
 * #endif
 * \endcode
 *
 * For version information at runtime
 * use the version methods in the TmdbQt namespace.
 */
#define LIBTMDBQT_IS_VERSION(a,b,c) ( LIBTMDBQT_VERSION >= LIBTMDBQT_MAKE_VERSION(a,b,c) )


namespace TmdbQt {
    /**
     * @brief Returns the major number of TmdbQt's version, e.g.
     * 1 for %TmdbQt 1.0.2.
     * @return the major version number at runtime.
     */
    TMDBQT_EXPORT unsigned int versionMajor();

    /**
     * @brief Returns the minor number of TmdbQt's version, e.g.
     * 0 for %TmdbQt 1.0.2.
     * @return the minor version number at runtime.
     */
    TMDBQT_EXPORT unsigned int versionMinor();

    /**
     * @brief Returns the release of TmdbQt's version, e.g.
     * 2 for %TmdbQt 1.0.2.
     * @return the release number at runtime.
     */
    TMDBQT_EXPORT unsigned int versionRelease();

    /**
     * @brief Returns the %TmdbQt version as string, e.g. "1.0.2".
     *
     * On contrary to the macro LIBTMDBQT_VERSION_STRING this function returns
     * the version number of TmdbQt at runtime.
     * @return the %TmdbQt version. You can keep the string forever
     */
    TMDBQT_EXPORT const char* versionString();
}

#endif
