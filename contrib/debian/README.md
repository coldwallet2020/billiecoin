
Debian
====================
This directory contains files used to package billiecoind/billiecoin-qt
for Debian-based Linux systems. If you compile billiecoind/billiecoin-qt yourself, there are some useful files here.

## billiecoin: URI support ##


billiecoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install billiecoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your billiecoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/billiecoin128.png` to `/usr/share/pixmaps`

billiecoin-qt.protocol (KDE)

