#include "include/stitching/matching/knn_matching.h"

#include <iostream>
#include <vector>

namespace stitching::matching {

void KnnMatching::init() { matcher = cv::DescriptorMatcher::create(matcherType); }

std::vector<cv::DMatch> KnnMatching::exec(cv::Ptr<core::IFeatures> features1, cv::Ptr<core::IFeatures> features2, int k) const
{

    if (features1.empty() || features2.empty()) {
         throw std::invalid_argument(std::string("Empty features!"));
    }

    std::vector<std::vector<cv::DMatch>> knnMatches;

    matcher->knnMatch(features1->descriptors, features2->descriptors, knnMatches, k);

    std::vector<cv::DMatch> matches{};

    for (auto& knnMatch : knnMatches) {
      if (knnMatch[0].distance < ratio * knnMatch[1].distance) {
        matches.push_back(knnMatch[0]);
      }
    }

    return matches;
}

void KnnMatching::free() { matcher.release(); }

void KnnMatching::setMatcherType(cv::DescriptorMatcher::MatcherType matcherType_) {
  KnnMatching::matcherType = matcherType_;
}

void KnnMatching::setRatio(const float & ratio_) { KnnMatching::ratio = ratio_; }

}