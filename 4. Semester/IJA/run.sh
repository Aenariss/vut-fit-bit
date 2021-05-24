#!/bin/sh
PATH_TO_FX=lib/javafx-sdk-11/lib
java -jar --module-path $PATH_TO_FX --add-modules javafx.controls,javafx.fxml dest/ija-app.jar