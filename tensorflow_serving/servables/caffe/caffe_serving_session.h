/* Copyright IBM Corp. All Rights Reserved. */
#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "caffe/proto/caffe.pb.h"

#include "tensorflow/core/lib/gtl/array_slice.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow_serving/servables/caffe/simple_thread_sink.h"
#include "tensorflow_serving/servables/tensorflow/serving_session.h"

namespace caffe {
// forward decl caffe::Net
template<class T> class Net;

// Caffe doesn't seem to provide an easy way to determine
// the network inputs/outputs without initializing one;
//
// This implementation is based on on caffe::Net::Init(...)
::tensorflow::Status ResolveNetInsOuts(const caffe::NetParameter& param,
                                       std::vector<std::string>& in_blobs,
                                       std::vector<std::string>& out_blobs);
} // namespace caffe

namespace tensorflow {
namespace serving {

struct CaffeSessionOptions {
  // force cpu-only mode, even when configured for GPU.
  bool force_cpu_only;
  // force the use of a particular gpu for this session.
  int force_gpu_id;

  CaffeSessionOptions();
  CaffeSessionOptions(bool inherit_defaults);

  // override default options for all sessions
  static CaffeSessionOptions& defaults();
};

// caffe model definition and class labels,
// and computed inputs and outputs
struct CaffeMetaGraphDef {
  caffe::NetParameter model_def;
  tensorflow::TensorProto classes;

  std::vector<string> resolved_inputs;
  std::vector<string> resolved_outputs;
};

// name of the output tensor which contains the class
// labels of the serving session (if any).
const char kClassLabelTensorName[] = "__labels__";

// Encapsulates a caffe network
class CaffeServingSession : public ServingSession {
 public:

  CaffeServingSession(const CaffeMetaGraphDef& graph,
                      const CaffeSessionOptions& opts);

  virtual ~CaffeServingSession();

  Status CopyTrainedLayersFromBinaryProto(const string trained_filename);

  virtual Status Run(const std::vector<std::pair<string, Tensor>>& inputs,
                     const std::vector<string>& output_tensor_names,
                     const std::vector<string>& target_node_names,
                     std::vector<Tensor>* outputs) override;

 private:
  Status Reshape(unsigned int batch_size);
  Status OutputClassLabels(std::vector<Tensor>* outputs);

  std::unique_ptr<caffe::Net<float>> net_;
  std::unique_ptr<Tensor> class_labels_;
  unsigned int batch_size_;

  std::unordered_map<string, unsigned int> input_blob_map_;
  std::unordered_map<string, unsigned int> output_blob_map_;
  SimpleThreadSink ts_;

  TF_DISALLOW_COPY_AND_ASSIGN(CaffeServingSession);
};

}  // namespace serving
}  // namespace tensorflow