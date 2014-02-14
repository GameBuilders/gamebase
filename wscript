import urllib2
import hashlib
import zipfile
from subprocess import call
from waflib.Task import Task
from waflib import TaskGen
from waflib.Build import BuildContext

class DownloadSource(Task):
    def run(self):
        src_http = urllib2.urlopen(self.url)
        src_archive = src_http.read()
        src_http.close()

        src_hasher = hashlib.sha256()
        src_hasher.update(src_archive)

        if self.hash_sha256 != src_hasher.hexdigest():
            raise Exception('hash mismatch: expected ' + self.hash_sha256 + ' got ' + src_hasher.hexdigest())

        self.outputs[0].write(src_archive, flags='wb')

class UnzipArchive(Task):
    def run(self):
        if self.env['DEST_OS'] == 'darwin':
            call(['mkdir', self.outputs[0].abspath()])
            call(['unzip', self.inputs[0].abspath(), '-d', self.outputs[0].abspath()])
        else:
            arc_zip = zipfile.ZipFile(self.inputs[0].abspath())
            arc_zip.extractall(self.outputs[0].abspath())


class SFGUIPatch(Task):
    def run(self):
        # We patch the SFGUI renderer to use GLEW instead of GLee
        sfgui_vbr_node = self.src_node.ant_glob('**/VertexBufferRenderer.cpp')[0]
        vbr = sfgui_vbr_node.read(flags='rb')
        vbr = vbr.replace('GLee.h', 'GL/glew.h').replace('GLEE', 'GLEW')
        sfgui_vbr_node.write(vbr, flags='wb')


def get_deps(ctx):
    deps = [
        {
            'name': 'box2d',
            'url': 'https://box2d.googlecode.com/files/Box2D_v2.2.1.zip',
            'hash': '63184a8ad317e19cd0c3408e14cd808816628b5c8686741dc32664c6f4db3a95'
        },
        {
            'name': 'freetype',
            'url': 'http://download.savannah.gnu.org/releases/freetype/ft252.zip',
            'hash': 'c68b1972788ddb8a84cc29a16fda0f85b9948a93db739a147b9ae98e671f39a1'
        },
        {
            'name': 'glew',
            'url': 'https://sourceforge.net/projects/glew/files/glew/1.10.0/glew-1.10.0.zip/download',
            'hash': '43c6229d787673ac1d35ebaad52dfdcc78c8b55d13ee78d8e4d7e4a6cb72b050'
        },
        {
            'name': 'sfml',
            'url': 'http://www.sfml-dev.org/download/sfml/2.1/SFML-2.1-sources.zip',
            'hash': '5f46d7748223be3f0c6a9fcf18c0016d227f7b1903cdbcd85f61ddbc82ef95bf',
        },
        {
            'name': 'sfgui',
            'url': 'http://sfgui.sfml-dev.de/download/29',
            'hash': '5eab368651eeb6207d2bcf8c3e280371c44d09186f3045a0a7933d30cff81a7a',
            'post': SFGUIPatch
        }
    ]
    src_nodes = {}

    deps_node = ctx.path.make_node('deps')
    deps_node.mkdir()
    for dep in deps:
        dep_name = dep['name']
        arcive_node = deps_node.make_node(dep_name + '_download')
        src_node = deps_node.make_node(dep_name + '_src')

        # Downloading Task
        dl_task = DownloadSource(env=ctx.env)
        dl_task.url = dep['url']
        dl_task.hash_sha256 = dep['hash']
        dl_task.set_outputs(arcive_node)
        ctx.add_to_group(dl_task)

        # Extraction Task
        unzip_task = UnzipArchive(env=ctx.env)
        unzip_task.set_inputs(arcive_node)
        unzip_task.set_outputs(src_node)
        ctx.add_to_group(unzip_task)

        if 'post' in dep:
            post_task = dep['post'](env=ctx.env)
            post_task.set_run_after(unzip_task)
            post_task.src_node = src_node
            ctx.add_to_group(post_task)

        src_nodes[dep_name] = src_node


class deps_ctx(BuildContext):
    cmd = 'get_deps'
    fun = 'get_deps'


def options(ctx):
    ctx.load('compiler_c compiler_cxx')


def configure(ctx):
    ctx.load('compiler_c compiler_cxx')
    if ctx.env['DEST_OS'] == 'linux':
        ctx.check_cfg(args='--cflags --libs', package='x11', uselib_store='X11', msg='Checking for X11')
        ctx.check_cfg(args='--cflags --libs', package='xrandr', uselib_store='XRANDR', msg='Checking for XRandR')
        ctx.check_cfg(args='--cflags --libs', package='openal', uselib_store='OPENAL', msg='Checking for OpenAL')
        ctx.check_cfg(args='--cflags --libs', package='sndfile', uselib_store='SNDFILE', msg='Checking for libsndfile')

# support for .mm files
@TaskGen.extension('.mm')
def mm_hook(self, node):
    "Bind the c++ file extensions to the creation of a :py:class:`waflib.Tools.cxx.cxx` instance"
    return self.create_compiled_task('mm', node)
 
class mm(Task):
    "Compile MM files into object files"
    run_str = '${CXX} ${ARCH_ST:ARCH} ${CXXFLAGS} ${FRAMEWORKPATH_ST:FRAMEWORKPATH} ${CPPPATH_ST:INCPATHS} ${DEFINES_ST:DEFINES} ${CXX_SRC_F}${SRC} ${CXX_TGT_F}${TGT}'
    vars    = ['CXXDEPS'] # unused variable to depend on, just in case
    ext_in  = ['.h'] # set the build order easily by using ext_out=['.h']

# support for .m files
@TaskGen.extension('.m')
def mm_hook(self, node):
    "Bind the c++ file extensions to the creation of a :py:class:`waflib.Tools.cxx.cxx` instance"
    return self.create_compiled_task('m', node)
 
class m(Task):
    "Compile M files into object files"
    run_str = '${CXX} ${ARCH_ST:ARCH} ${CXXFLAGS} ${FRAMEWORKPATH_ST:FRAMEWORKPATH} ${CPPPATH_ST:INCPATHS} ${DEFINES_ST:DEFINES} ${CXX_SRC_F}${SRC} ${CXX_TGT_F}${TGT}'
    vars    = ['CXXDEPS'] # unused variable to depend on, just in case
    ext_in  = ['.h'] # set the build order easily by using ext_out=['.h']

def build(ctx):
    deps_node = ctx.path.make_node('deps')

    # Box2D build
    box2d_node = deps_node.make_node('box2d_src').make_node('Box2D_v2.2.1')
    box2d_sources = box2d_node.ant_glob('Box2D/**/*.cpp')
    box2d_include_node = box2d_node
    ctx.stlib(
        source       = box2d_sources,
        target       = 'box2d',
        includes     = [box2d_include_node],
        cxxflags       = ['-w', '-O3'],
    )

    # Freetype build
    freetype_node = deps_node.make_node('freetype_src').make_node('freetype-2.5.2')
    freetype_source = ['src/base/ftsystem.c', 'src/base/ftinit.c', 'src/base/ftdebug.c', 'src/base/ftbase.c', 'src/base/ftbbox.c', 'src/base/ftglyph.c', 'src/base/ftbdf.c', 'src/base/ftbitmap.c', 'src/base/ftcid.c', 'src/base/ftfstype.c', 'src/base/ftgasp.c', 'src/base/ftgxval.c', 'src/base/ftlcdfil.c', 'src/base/ftmm.c', 'src/base/ftotval.c', 'src/base/ftpatent.c', 'src/base/ftpfr.c', 'src/base/ftstroke.c', 'src/base/ftsynth.c', 'src/base/fttype1.c', 'src/base/ftwinfnt.c', 'src/base/ftxf86.c', 'src/bdf/bdf.c', 'src/cff/cff.c', 'src/cid/type1cid.c', 'src/pcf/pcf.c', 'src/pfr/pfr.c', 'src/sfnt/sfnt.c', 'src/truetype/truetype.c', 'src/type1/type1.c', 'src/type42/type42.c', 'src/winfonts/winfnt.c', 'src/raster/raster.c', 'src/smooth/smooth.c', 'src/autofit/autofit.c', 'src/cache/ftcache.c', 'src/gzip/ftgzip.c', 'src/lzw/ftlzw.c', 'src/bzip2/ftbzip2.c', 'src/gxvalid/gxvalid.c', 'src/otvalid/otvalid.c', 'src/psaux/psaux.c', 'src/pshinter/pshinter.c', 'src/psnames/psnames.c']
    freetype_source = [freetype_node.make_node(source) for source in freetype_source]
    freetype_include_node = freetype_node.make_node('include')
    ctx.stlib(
        source       = freetype_source,
        target       = 'freetype',
        includes     = [freetype_include_node],
        cflags       = ['-w', '-O3'],
        defines      = 'FT2_BUILD_LIBRARY'
    )

    # GLEW build
    glew_node = deps_node.make_node('glew_src').make_node('glew-1.10.0')
    glew_include_node = glew_node.make_node('include')
    ctx.stlib(
        source       = glew_node.ant_glob('src/glew.c'),
        target       = 'glew',
        includes     = [glew_include_node],
        cflags       = ['-w', '-O3'],
        defines      = 'GLEW_STATIC'
    )

    # SFML build
    sfml_node =  deps_node.make_node('sfml_src').make_node('SFML-2.1')
    sfml_source = sfml_node.ant_glob('src/SFML/Audio/*.cpp') + \
                  sfml_node.ant_glob('src/SFML/Graphics/**/*.cpp') + \
                  sfml_node.ant_glob('src/SFML/System/*.cpp') + \
                  sfml_node.ant_glob('src/SFML/Window/*.cpp')
    sfml_includes =  ['include', 'src']
    sfml_uselibs = []
    if ctx.env['DEST_OS'] == 'linux':
        sfml_source = sfml_source + sfml_node.ant_glob('src/SFML/System/Unix/*.cpp') + \
                                    sfml_node.ant_glob('src/SFML/Window/Linux/*.cpp')
        sfml_uselibs.append('OPENAL')
    if ctx.env['DEST_OS'] == 'win32':
        sfml_source = sfml_source + sfml_node.ant_glob('src/SFML/System/Win32/*.cpp') + \
                                    sfml_node.ant_glob('src/SFML/Window/Win32/*.cpp')
        sfml_includes.extend(['extlibs/headers/AL', 'extlibs/headers', 'extlibs/headers/libsndfile/windows', 'extlibs/headers/jpeg', 'extlibs/headers/libfreetype/windows'])
    if ctx.env['DEST_OS'] == 'darwin':
        sfml_source = sfml_source + sfml_node.ant_glob('src/SFML/System/Unix/*.cpp') + \
                                    sfml_node.ant_glob('src/SFML/Window/OSX/*.cpp') + \
                                    sfml_node.ant_glob('src/SFML/Window/OSX/*.mm') + \
                                    sfml_node.ant_glob('src/SFML/Window/OSX/*.m')
        sfml_includes.extend(['extlibs/headers/AL', 'extlibs/headers', 'extlibs/headers/libsndfile/osx', 'extlibs/headers/jpeg', 'extlibs/headers/libfreetype/osx'])

    sfml_includes = [sfml_node.abspath() + '/' + include for include in sfml_includes]
    sfml_includes = [freetype_include_node, glew_include_node] + sfml_includes
    ctx.stlib(
        source       = sfml_source,
        target       = 'sfml',
        defines      = 'SFML_STATIC UNICODE _UNICODE GLEW_STATIC',
        includes     = sfml_includes,
        uselib       = sfml_uselibs,
        cxxflags     = ['-w', '-O3']
    )

    # SFGUI build
    sfgui_node =  deps_node.make_node('sfgui_src').make_node('SFGUI-0.2.0')
    sfgui_source = sfgui_node.ant_glob('src/**/*.cpp')
    sfgui_includes =  ['include', 'extlibs/libELL/include']
    sfgui_includes = [sfgui_node.abspath() + '/' + include for include in sfgui_includes]
    sfgui_includes = [glew_include_node, sfml_node.make_node('include')] + sfgui_includes
    ctx.stlib(
        source       = sfgui_source,
        target       = 'sfgui',
        defines      = ['SFGUI_INCLUDE_FONT', 'SFGUI_STATIC', 'SFML_STATIC', 'UNICODE', '_UNICODE', 'GLEW_STATIC'],
        includes     = sfgui_includes,
        cxxflags     = ['--std=c++11', '-w', '-O3']
    )

    sources = ['main.cpp']
    uselibs = []
    defines = ['SFGUI_STATIC', 'SFML_STATIC', 'UNICODE', '_UNICODE', 'GLEW_STATIC']
    libs = []
    frameworks = []
    frameworksPath = []
    stlibs = []
    stlibpath = []
    includes = [box2d_include_node, glew_include_node, sfml_node.make_node('include'), sfgui_node.make_node('include')]

    if ctx.env['DEST_OS'] == 'linux':
        uselibs.extend(['X11', 'XRANDR', 'OPENAL', 'SNDFILE'])
        libs.extend(['dl', 'GL', 'jpeg', 'sndfile'])
    if ctx.env['DEST_OS'] == 'win32':
        stlibpath.append(sfml_node.abspath() + '/extlibs/libs-mingw/x86/')
        libs.extend(['opengl32', 'Gdi32', 'Winmm', 'jpeg', 'sndfile'])
    if ctx.env['DEST_OS'] == 'darwin':
        stlibpath.append(sfml_node.abspath() + '/extlibs/libs-osx/lib')
        libs.extend(['jpeg'])
        frameworksPath.append(sfml_node.abspath() + '/extlibs/libs-osx/Frameworks')
        frameworks.extend(['OpenAL', 'OpenGL', 'freetype', 'sndfile', 'IOKit', 'Carbon', 'AppKit', 'Foundation'])

    targets = [
        ('ben', 'example/main.cpp')
    ]

    for target in targets:
        ctx.program(
            source       = target[1],
            target       = target[0],
            use          = ['box2d', 'sfgui', 'sfml', 'freetype', 'glew'],
            uselib       = uselibs,

            defines      = defines,

            includes     = includes,

            lib          = libs,
            frameworkpath= frameworksPath,
            framework    = frameworks,

            stlib        = stlibs,
            stlibpath    = stlibpath,

            linkflags    = [],

            cxxflags     = ['-Wall', '-O0', '-g', '--std=c++11']
        )
