// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

// A type location has a name, type, export/import, and location
struct TypeLocation {
	std::string name;
	std::string parent;
	std::string type;
	std::string reg;
	std::string exportOrImport;
	std::string locoffset;
	std::string location;
};
