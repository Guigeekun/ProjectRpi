# ProjectRpi
Synth thing for RaspberryPi

## Install

Donc notre lib audio sera sox http://sox.sourceforge.net/

    sudo apt-get install sox libsox-fmt-all bc

note: sur la raspberry, la version 14.4.2 de sox ne détecte pas de périphérique audio, on a donc installé la version 14.4.1

======================================================
Tous ce qui concerne PortAudio est OUTDATED : je le laisse juste en "souvenir"

http://portaudio.com/docs/v19-doxydocs/compile_linux.html

Setup de portAudio 

    ./configure --without-jack --without-oss --with-alsa

Ici la configuration devrait indiquer ALSA : YES sinon faites

    sudo apt-get install libasound-dev

    make
    sudo make install

Maintenant copiez portaudio.a

    cp /usr/local/lib/libportaudio.a /YOUR/PROJECT/DIR

Et portAudio.h localisé dans /include
Pour compiler :

    gcc rpi.c libportaudio.a -lrt -lm -lasound -pthread -o rpi

# Kézako

un synthé programmable permettant de mapper des notes à des bouton (voir des accords entier)

# Cahier des Charges

Sortir un son recomposé (spectre harmonique théorique)
deux trois détails sur le spectre théorique :
fondamental à amplitude 100
décroissance en 1/x à chaque fréq*x

permettre via un fichier config de mapper les notes aux boutons (GPIO w/ WiringPi)

 Permettre la gestion des accords (donc mapper un accords à un bouton)

(optionel) calcul des accords (on se limite aux “Notes Majeur/mineur x” donc pas de SUS et de truc du genre)

(optionel) si vraiment on a le temps on peut permettre de sampler (là ça devient compliqué parsqu’il faut pouvoir décoder un son et ensuite l’adapter, c’est très maths et j’ai aucune idée de comment faire ça)

bon on pourrait rajouter des features mais c’est déjà pas mal


## TODO (en dehors des features à dev) :
test la sortie audio (ça devrait pas poser de prob)
créer une map des boutons

## Lib :
PortAudio
faudra rajouter la lib pour décoder si on va jusque là
wiringPi pour l'utilisation des ports GPIO

## infos raspberry:
login: pi
password: raspberrypi

erreur lors de l'utilisation de sox:
can’t open output file ‘/dev/dsp’:unable to reboot OSS driver. possibly accessing an invalid file/device