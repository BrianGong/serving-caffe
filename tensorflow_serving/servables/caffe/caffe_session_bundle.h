/* Copyright 2016 IBM Corp. All Rights Reserved. */
#pragma once

#include <memory>

#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow_serving/servables/caffe/caffe_serving_session.h"

#include "tensorflow/core/framework/tensor.pb.h"

namespace tensorflow {
namespace serving {

// Low-level functionality for setting up a Caffe inference Session.
const char kGraphDefFilename[] = "deploy.prototxt";
const char kVariablesFilename[] = "weights.caffemodel";
const char kGraphTxtLabelFilename[] = "classlabels.txt";

// A global initialization function that you should call in your main function.
// Currently it just invokes caffe::GlobalInit(..)
void CaffeGlobalInit(int* pargc, char*** pargv);

// Very roughly equivalent to a TF session bundle
struct CaffeSessionBundle {
  std::unique_ptr<tensorflow::Session> session;
  CaffeMetaGraphDef meta_graph_def;
};

// Loads a manifest and initialized session using the output of an Exporter
tensorflow::Status LoadSessionBundleFromPath(
    const CaffeSessionOptions& options,
    const tensorflow::StringPiece export_dir,
    CaffeSessionBundle* bundle);

}  // namespace serving
}  // namespace tensorflow
