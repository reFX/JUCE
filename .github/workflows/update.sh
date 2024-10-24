#!/bin/sh -e

echo ** Move to repo
cd $RUNNER_WORKSPACE/JUCE
pwd

git config --global user.email "figbug@gmail.com"
git config --global user.name "Roland Rabien"

echo ** Updating reFX/JUCE from juce-framework/JUCE

git checkout develop

echo ** Setup remote
git remote -v
git remote add upstream https://github.com/juce-framework/JUCE.git
git remote -v

echo ** Fetch remote
git fetch upstream

echo ** Update develop
git merge upstream/develop
git push

echo ** Update reFX/develop
git checkout reFX/develop
git merge develop
git push