# PBXBuild
Command line tool to build xcode projects on any platform

###### This project is under active development and subject to breaking changes.

* How to build: `make`
* How to use (quick start): `pbxbuild -project MySuperProject.xcodeproj -target MySuperApp`

## Features

- Build Xcode projects on OS X and Linux
	- Products and Intermediates can be found in:
		- Linux: `~/.pbxbuild/DerivedData/<project_name>/`
		- OS X: `~/Application Support/PBXBuild/DerivedData/<project_name>/`

## Additional features

As Xcode projects are not meant for multiplatform projects at first, PBXBuild introduces additional configuration options.
To use them, just create a custom build setting in the properties of your target. Additional settings include:

- PBXBUILD_CFLAGS, PBXBUILD_CXXFLAGS, PBXBUILD_LINKER_OPTIONS (json strings to specify platform-dependent options):  
	`{ "common": ["-g"], "linux": ["-ldl"], "osx": ["-framework", "Foundation"] }`

## Roadmap

- Get rid of the pstream library for better error handling
- Improvement to platform-dependent code
- Support for the GCC toolchain
- Concurrent builds
- User specified target CPU architecture
- Support for fat binaries
- Processing of Resource files
- Better support of OS X application targets
- Support for iOS and Android targets
- Support for Windows hosts and targets
- Support for the Swift language
- User and code documentation

Contributions are welcome ! :)
