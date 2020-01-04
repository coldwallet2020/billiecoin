Sample init scripts and service configuration for billiecoind
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/billiecoind.service:    systemd service unit configuration
    contrib/init/billiecoind.openrc:     OpenRC compatible SysV style init script
    contrib/init/billiecoind.openrcconf: OpenRC conf.d file
    contrib/init/billiecoind.conf:       Upstart service configuration file
    contrib/init/billiecoind.init:       CentOS compatible SysV style init script

1. Service User
---------------------------------

All three Linux startup configurations assume the existence of a "billiecoincore" user
and group.  They must be created before attempting to use these scripts.
The OS X configuration assumes billiecoind will be set up for the current user.

2. Configuration
---------------------------------

At a bare minimum, billiecoind requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, billiecoind will shutdown promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that billiecoind and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If billiecoind is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running billiecoind without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see `contrib/debian/examples/billiecoin.conf`.

3. Paths
---------------------------------

3a) Linux

All three configurations assume several paths that might need to be adjusted.

Binary:              `/usr/bin/billiecoind`  
Configuration file:  `/etc/billiecoincore/billiecoin.conf`  
Data directory:      `/var/lib/billiecoind`  
PID file:            `/var/run/billiecoind/billiecoind.pid` (OpenRC and Upstart) or `/var/lib/billiecoind/billiecoind.pid` (systemd)  
Lock file:           `/var/lock/subsys/billiecoind` (CentOS)  

The configuration file, PID directory (if applicable) and data directory
should all be owned by the billiecoincore user and group.  It is advised for security
reasons to make the configuration file and data directory only readable by the
billiecoincore user and group.  Access to billiecoin-cli and other billiecoind rpc clients
can then be controlled by group membership.

3b) Mac OS X

Binary:              `/usr/local/bin/billiecoind`  
Configuration file:  `~/Library/Application Support/BilliecoinCore/billiecoin.conf`  
Data directory:      `~/Library/Application Support/BilliecoinCore`
Lock file:           `~/Library/Application Support/BilliecoinCore/.lock`

4. Installing Service Configuration
-----------------------------------

4a) systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start billiecoind` and to enable for system startup run
`systemctl enable billiecoind`

4b) OpenRC

Rename billiecoind.openrc to billiecoind and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/billiecoind start` and configure it to run on startup with
`rc-update add billiecoind`

4c) Upstart (for Debian/Ubuntu based distributions)

Drop billiecoind.conf in /etc/init.  Test by running `service billiecoind start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

4d) CentOS

Copy billiecoind.init to /etc/init.d/billiecoind. Test by running `service billiecoind start`.

Using this script, you can adjust the path and flags to the billiecoind program by
setting the SYSD and FLAGS environment variables in the file
/etc/sysconfig/billiecoind. You can also use the DAEMONOPTS environment variable here.

4e) Mac OS X

Copy org.billiecoin.billiecoind.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.billiecoin.billiecoind.plist`.

This Launch Agent will cause billiecoind to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run billiecoind as the current user.
You will need to modify org.billiecoin.billiecoind.plist if you intend to use it as a
Launch Daemon with a dedicated billiecoincore user.

5. Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.
