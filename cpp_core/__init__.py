def load_cpp_scripts(**kwargs):
    import cppyy, subprocess, os
    original_dir = os.getcwd()
    script_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(script_dir)

    include_dir = 'include'
    src_dir = 'src'
    scripts = ['board', 'iterator', 'evidence', 'pattern']
    library = "shared.so"

    if not os.path.exists(library) or 'rebuild' in kwargs:
        print("Building the cpp library")
        script_locations = ' '.join(map(lambda scr: f'{src_dir}/{scr}.cpp', scripts))
        command = f'g++ -std=c++17 -shared -o {library} -I{include_dir} {script_locations} -fPIC'
        process = subprocess.run(command, capture_output=True, text=True, shell=True)
        if 'debug' in kwargs: 
            print(f'Output: {process.stdout}\nError: {process.stderr}')
        print("Done")
    
    cppyy.load_library(library)
    for script in scripts:
        cppyy.include(f'{include_dir}/{script}.h')

    os.chdir(original_dir)
    return cppyy.gbl

def cpp_iterator(iterator):
    iterator.init()
    while iterator.hasNext():
        yield tuple(iterator.getNext())

def create_null(class_name):
    from cppyy import bind_object, nullptr
    return bind_object(nullptr, class_name)