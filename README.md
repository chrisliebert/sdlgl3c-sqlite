# sdlgl3c-sqlite
A 3D OpenGL 3 renderer written in C that loads 3D models from an sqlite3 database.
This project is based of https://github.com/chrisliebert/sdlgl3-wavefront although the renderer
is written in C rather than C++. C++ and https://github.com/syoyo/tinyobjloader are used in the obj2sqlite
sub-project which stores Wavefront data in an SQLite database. The C-renderer has performed faster and more
efficiently than the C++ version(sdlgl3-wavefront) due to excessive use of std::string::copy (major bottleneck indicated by gprof).
The use of SQLite make it easier to store custom attributes in the database
and load them from the C renderer, increasing the overall robustness.