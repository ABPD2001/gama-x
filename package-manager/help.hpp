#ifndef HELP_HPP
#define HELP_HPP
constexpr const char help[] = R"(Usage: gxpm [FLAGS]... [VERBS]... [ARGUMENTS/OPTIONS]...
Package manager for Gama-X, manages repositories of Gama-X.

Flags:
    [-u, --username]: set username of target user, this is required if youre running on admin premision.
    [-v, --verbose]: verbose mode.

Verbs:
    start               initalizes empty repository (creates empty metadata file).
    help                prints this page.
    version             prints version of package manager.
    validate            validates (checks for chunk segmention errors) a full repository.
    mergable            checks if two or more repositories are mergable.
    create-mainpoint:   creates mainpoint for a library.
        [-r, --repository]:          repository to use for including dependecies and librarires.
        [-nd, --ignore-dependecies]: skips process of navigating dependecies.
        [-nr, --ignore-repository]:  skips process of adding libraries from repository.
        [-o, --output]:              output file.

    setup               setups configurations of Gama-X Package Manger (must run with root premission).
        [-l, --local]: setup locally.
        [-nl, --skip-locals]: skip local configurations and setups.
        
    merge               merges two or more repositories.
        [-n, --name]: name of output repository.
        [-p, --path]: path of output repository directory.

    reset      resets configurations (must run with root premission).
        [-r, --rage]:           deletes global repositories also.
        [-l, --affect-local]:   empties local repositories bank also.

    edit:       gxpm edit <'lib'/'repo'> <ARGS> <OPTIONS>
        repo    edits repositories metadata file of package manager.
        lib     edits metadata file of a repository.
            [-r, --repository]:         from which repository.
            [-V, --version]:            set version of library.
            [-dp, --dependecies]:        set dependecies of library.
            [-m, --mainpoint-relative]: set relative to library directory mainpoint file path.

        [-p, --path]:       edit library/repository path (note: doesnt move automatically, just registers).
        [-n, --name]:       edit library/repository name.
        [-d, --desciption]: edit library/repository desciption (.txt file path).

    remove:     gxpm remove <'lib'/'repo'> <ARGS> <OPTIONS>
        repo    remove a repository.
        lib     remove a library from repository.
            [-r, --repository]:     from which repository.
             
  
    add:    gxpm add <'lib'/'repo'> <PATH> <OPTIONS>
        repo    add new repository.
        lib     insert a library into a repository.
            [-r, --repository]:         to which repository.
            [-p, --dependecies]:        dependecies of library.
            [-V, --lib-version]:            version of library.
            [-m, --mainpoint-relative]: relative to library directory mainpoint file path.

        [-n, --name]:           name of library/repository.
        [-d, --description]:    description of library/repository.
        
    list:   gxpm list <'lib'/'repo'> <ARGS> <OPTIONS>
        lib    lists available libraries (argument required).
        repo   lists available repositories.
            [-r, --repository]: from which repository.

    copy:   gxpm copy <'lib'/'repo'> <ARGS> <OPTIONS>
        repo    copies a repository into other location.
        lib    copies libraries from a repository into another repository.
            [-r, --repository]:     from which repository.
        
        [-t, --target]: determine where to copy.

Note: if youre merging repositories, you must becareful of conflicts, package manager cannot solve merging conflicts automatically!
Note: after rage reset, its required to initialize GXPM again.

<--- GXPM Version 1.0.0 --->)";

#endif