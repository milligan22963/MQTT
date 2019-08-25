# MQTT
A C++ mqtt library


Base implementation of MQTT library supporting all messaging and persistence*.

My plan is to get the protocol solid under non-ssl traffic then add in openssl to support that aspect.

Targetting the 3.1.1 version initially.

Currently connect, subscribe, and publish are working.

Dependent upon afmSocket which is another repository I have.
Will be combined into one package.

* Features are forthcoming - i.e. work in progress.
