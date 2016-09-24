/*
 * Copyright (c) 2016, Egor Pugin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of
 *        its contributors may be used to endorse or promote products
 *        derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "common.h"
#include "project_path.h"

struct Directories;

struct Package
{
    ProjectPath ppath;
    Version version;
    ProjectFlags flags;

    path getDirSrc() const;
    path getDirObj() const;
    String getHash() const;
    path getHashPath() const;
    path getStampFilename() const;

    bool empty() const { return ppath.empty() || !version.isValid(); }
    bool operator<(const Package &rhs) const { return std::tie(ppath, version) < std::tie(rhs.ppath, rhs.version); }

    // misc data
    String target_name;
    String variable_name;

    void createNames();
    String getTargetName() const;
    String getVariableName() const;
};

using Packages = std::map<String, Package>;

using PackageIndex = std::map<String, path>;
using PackageDependenciesIndex = std::map<String, String>;

Package extractFromString(const String &target);

PackageIndex readPackagesIndex(const path &dir);
void writePackagesIndex(const path &dir, const PackageIndex &idx);

PackageDependenciesIndex readPackageDependenciesIndex(const path &dir);
void writePackageDependenciesIndex(const path &dir, const PackageDependenciesIndex &idx);

struct CleanTarget
{
    enum Type
    {
        None = 0b0000'0000,

        Src = 0b0000'0001,
        Obj = 0b0000'0010,
        Lib = 0b0000'0100,
        Bin = 0b0000'1000,

        All = Src | Obj | Lib | Bin,
        AllExceptSrc = Obj | Lib | Bin,
    };
};

void cleanPackages(const String &s, int flags = CleanTarget::All);
