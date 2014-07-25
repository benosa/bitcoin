
Debian
====================
This directory contains files used to package thalercoind/thalercoin-qt
for Debian-based Linux systems. If you compile thalercoind/thalercoin-qt yourself, there are some useful files here.

## thalercoin: URI support ##


thalercoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install thalercoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your thalercoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/thalercoin128.png` to `/usr/share/pixmaps`

thalercoin-qt.protocol (KDE)

