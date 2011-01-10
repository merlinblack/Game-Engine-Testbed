#! /bin/bash

for x in *.xml; do
    OgreXMLConverter $x ../$(basename $x .xml)
done
