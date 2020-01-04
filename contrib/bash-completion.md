Bash Completion
---------------------

The following script provide bash completion functionality for `sycoind` and `billiecoin-cli`.

* `contrib/billiecoind.bash-completion`
* `contrib/billiecoin-cli.bash-completion`
* `contrib/_osx.bash-completion`

### OSX ###
Use `brew` to install `bash-completion` then update `~/.bashrc` to include the completion scripts and helper functions to provide `have()` and `_have()` on OSX.

The example assumes Billiecoin was compiled in `~/billiecoin` and the scripts will be availabe in `~/billiecoin/contrib`, however they can be moved to a different location.

```
brew install bash bash-completion
sudo bash -c 'echo /usr/local/bin/bash >> /etc/shells'
chsh -s /usr/local/bin/bash

BASHRC=$(cat <<EOF
if [ -f $(brew --prefix)/etc/bash_completion ]; then
  . $(brew --prefix)/etc/bash_completion
fi
. ~/billiecoin/contrib/_osx.bash-completion
. ~/billiecoin/contrib/billiecoind.bash-completion
. ~/billiecoin/contrib/billiecoin-cli.bash-completion
EOF
)

grep -q "/etc/bash_completion" ~/.bashrc || echo "$BASHRC" >> ~/.bashrc
. ~/.bashrc

```