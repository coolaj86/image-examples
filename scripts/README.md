Installation
====

You may install these files on a regular user account like so:

    cd ~/
    git clone git://github.com/coolaj86/image-examples.git
    rsync -avh ~/image-examples/scripts/ ~/local/bin/

    mkdir ~/local/bin -p
    echo 'PATH=~/local/bin/:${PATH}' >> ~/.bashrc
    source ~/.bashrc

Dependencies
====

  * `bash`
  * `ffmpeg`
