#!/bin/bash

set -e
set -u

commit=`git rev-parse --verify HEAD`

original_dir="$PWD"

working_dir="/tmp"

site_dir="$working_dir/libpx_site"

time_stamp=`date +%F--%H:%M:%S`

release_stamp=$time_stamp--$commit

editor_src_dir="$PWD/emscripten-build/editor"
editor_dst_dir="$site_dir/editor/$release_stamp"

doc_src_dir="$PWD/doxygen"
doc_dst_dir="$site_dir/doxygen/$release_stamp"

if [ ! -e "$editor_src_dir/pxedit_sdl.html" ]; then
  echo "Cannot find webpage."
  exit 1
fi

if [ -e "$site_dir" ]; then
  echo "Site directory already exists."
  exit 1
fi

doxygen

git clone git@github.com:tay10r/libpx_site.git "$site_dir" && cd "$site_dir"

mkdir -p "$doc_dst_dir" "$editor_dst_dir"

cp "$editor_src_dir/pxedit_sdl.html" "$editor_dst_dir"
cp "$editor_src_dir/pxedit_sdl.js"   "$editor_dst_dir"
cp "$editor_src_dir/pxedit_sdl.wasm" "$editor_dst_dir"

cp -R "$doc_src_dir/html/"* "$doc_dst_dir"

ln -s "editor/$release_stamp" "editor/latest"

ln -s "doxygen/$release_stamp" "doxygen/latest"

cat >index.md << EOF
libpx
=====

This webpage contains the API documentation and the online editor of the libpx project.

Last updated $(date).

Using commit [$commit](https://github.com/tay10r/libpx/tree/$commit).

Visit [here](doxygen/latest/index.html) for the latest documentation.

Visit [here](editor/latest/pxedit_sdl.html) to use the latest editor release.
EOF

git add .

git commit -F - << EOF
New release from deployment script.

Commit: $commit
EOF

git push origin master

cd "$original_dir"

rm -Rf "$site_dir"
