Installation
====

You may install these files on a regular user account like so:

Create a local `bin` and place it in your path:

    mkdir ~/local/bin -p
    echo 'PATH=~/local/bin/:${PATH}' >> ~/.bashrc
    source ~/.bashrc

Copy the executable scripts into that path:

    cd ~/
    git clone git://github.com/coolaj86/image-examples.git
    rsync -avh ~/image-examples/scripts/ ~/local/bin/

Dependencies
====

  * `bash`
  * `ffmpeg`
