# (c) 2022 Oleksandr Kolodkin <alexandr.kolodkin@gmail.com>
# This code is licensed under MIT license (see LICENSE.txt for details)

try:
    import gzip
    import jsmin
    import pathlib
    import htmlmin
    import csscompressor
except ImportError:
    print(f"Name = {__name__}")
    if __name__ != "__main__":
        Import("env")
        env.Execute("$PYTHONEXE -m pip install jsmin")
        env.Execute("$PYTHONEXE -m pip install htmlmin")
        env.Execute("$PYTHONEXE -m pip install csscompressor")
        import jsmin
        import htmlmin
        import csscompressor


def compress(source, destination):
    print(f"Compressing '{source}' to '{destination}' ...")
    pathlib.Path(destination).parent.mkdir(parents=True, exist_ok=True)

    with open(source, 'rb') as input:
        with gzip.open(destination, 'wb') as output:
            output.write(input.read())


def minimize_html(source, destination):
    print(f"Minimizing '{source}' to '{destination}' ...")
    with open(source, 'r') as input:
        with open(destination, 'w') as output:
            output.write(htmlmin.minify(input.read(),
                remove_comments=True,
                remove_empty_space=True,
                remove_all_empty_space=True,
                reduce_empty_attributes=True,
                reduce_boolean_attributes=True,
                remove_optional_attribute_quotes=True,
            ))


def minimize_css(source, destination):
    print(f"Minimizing '{source}' to '{destination}' ...")
    with open(source, 'r') as input:
        with open(destination, 'w') as output:
            output.write(csscompressor.compress(input.read()))


def minimize_js(source, destination):
    print(f"Minimizing '{source}' to '{destination}' ...")
    with open(source, 'r') as input:
        with open(destination, 'w') as output:
            output.write(jsmin.jsmin(input.read()))


def minimize(source):
    path = pathlib.Path(source)

    if path.suffix == '.htm':
        min_destination = str(path.with_suffix('.min.htm'))
        minimize_html('data_src/' + source, 'data_src/' + min_destination)
        compress('data_src/' + min_destination, 'data/portal/' + source + '.gz')

    elif path.suffix == '.html':
        min_destination = str(path.with_suffix('.min.html'))
        minimize_html('data_src/' + source, 'data_src/' + min_destination)
        compress('data_src/' + min_destination, 'data/portal/' + source + '.gz')

    elif path.suffix == '.css':
        min_destination = str(path.with_suffix('.min.css'))
        minimize_css('data_src/' + source, 'data_src/' + min_destination)
        compress('data_src/' + min_destination, 'data/portal/' + source + '.gz')

    elif path.suffix == '.js':
        min_destination = str(path.with_suffix('.min.js'))
        minimize_js('data_src/' + source, 'data_src/' + min_destination)
        compress('data_src/' + min_destination, 'data/portal/' + source + '.gz')
    
    else:
        compress('data_src/' + source, 'data/portal/' + source + '.gz')


def build_web(*sources):
    print("Building web ...")
    for source in sources:
        minimize(source)


def prebuild(source, target, env):
    print('[COPY/GZIP DATA FILES]')
    build_web(
        'index.html',
        'script.js',
        'settings.html',
        'styles.css',
        'system.html',
        'wifi.html',
        'static/favicon.png',
        'static/logo.svg',
    )
    print('[COPY/GZIP DATA FILES]')


if __name__ == "__main__":
    prebuild(1,2,3)
else:
    Import("env")
    env.AddPreAction('$BUILD_DIR/littlefs.bin', prebuild)
