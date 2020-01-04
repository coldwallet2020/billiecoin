#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
BUILDDIR=${BUILDDIR:-$TOPDIR}

BINDIR=${BINDIR:-$BUILDDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

BILLIECOIND=${BILLIECOIND:-$BINDIR/billiecoind}
BILLIECOINCLI=${BILLIECOINCLI:-$BINDIR/billiecoin-cli}
BILLIECOINTX=${BILLIECOINTX:-$BINDIR/billiecoin-tx}
BILLIECOINQT=${BILLIECOINQT:-$BINDIR/qt/billiecoin-qt}

[ ! -x $BILLIECOIND ] && echo "$BILLIECOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
BTCVER=($($BILLIECOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for billiecoind if --version-string is not set,
# but has different outcomes for billiecoin-qt and billiecoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$BILLIECOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $BILLIECOIND $BILLIECOINCLI $BILLIECOINTX $BILLIECOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
