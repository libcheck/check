#!/bin/sh
rpm --define "_topdir /home/amalec/src/check/rpm" -ba SPECS/check.spec --nodeps
