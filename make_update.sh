#!/bin/bash

rm camUpdate/update.md5sum
md5sum camUpdate/update* camUpdate/Chronos1_4PowerController.X.production.hex camUpdate/debian.img.gz > camUpdate/update.md5sum
