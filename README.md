def2fgd is tool for converting .def and .ent files used by GtkRadiant and Netradiant to .fgd files used by J.A.C.K.

# Usage

Open terminal and type:

    /path/to/def2fgd input-file output-file

On Windows open cmd.exe and type:

    path\to\def2fgd.exe input-file output-file

Where input-file is path to .def or .ent file and output-file is name for generated fgd file. 
Generated fgd should be suitable for loading in J.A.C.K., but it's still not perfect due to some fundamental differences between formats. Besides J.A.C.K. fgd can provide information not provided by Radiant .def or .ent files at all. So some handwork is still needed if you want to make the fgd file better. See [this list](https://bitbucket.org/FreeSlave/def2fgd/wiki/Refined%20fgd%20files) for already modified 'good' fgd files.

def2fgd also has some options for automated adding of offset and bobbing parameters for point entities. To get help run:

    def2fgd -help

# Download def2fgd

See [Downloads page](https://bitbucket.org/FreeSlave/def2fgd/wiki/def2fgd%20downloads) to check if there's prebuilt package for your platform.

# Build from source

If you want to build def2fgd from source read [instructions](https://bitbucket.org/FreeSlave/def2fgd/wiki/Building%20from%20source)
