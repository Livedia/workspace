#! /bin/bash

if [ $# -ne 2 ]
then echo "param is not 2, is $#"
fi

packg=$1
node=$2


function BuildAndRun(){
  bazel build $1/$packg:$node



  if [ $? -eq 0 ] ; then
    echo "build $node successful !"
    if [ -f "exe_$node" ] ; then
      echo "rm the outdate file: $node"
      rm -f "exe_$node"
    fi
    echo "---- run $node, the result is below: ----"
    cp bazel-bin/$1/$packg/$node "exe_$node" && ./"exe_$node"
  fi
  return 0
}

BuildAndRun study_bazel
