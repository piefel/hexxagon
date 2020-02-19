#!/bin/sh

echo "Running aclocal..."
aclocal
echo "Running autoheader..."
autoheader
echo "Running automake..."
automake
echo "Running autoconf..."
autoconf
echo "Done!"
