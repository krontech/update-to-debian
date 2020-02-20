#!/bin/bash

cd camUpdate
rm update.md5sum
md5sum update* Chronos1_4PowerController.X.production.hex debian.img.gz > update.md5sum
