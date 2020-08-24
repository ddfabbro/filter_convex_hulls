// This code follows Google C++ Style Guide.

// Eliminate overlapped convex hulls
// - Load convex_hulls.json.
// - Some of the convex hulls stored in convex_hulls.json are overlapped.
// - Eliminate the overlapped convex hulls based on the following rule.
//   - Eliminate if the convex hull has more than 50% its own area overlapped with
//     other convex hulls.

// - Generate result_convex_hulls.json which stores only the remaining (non-eliminated)
//   convex hulls. The format should be the same as convex_hulls.json.
// - Authorized program language is only C++.
//   - Follow a coding style guide which you are using.
//     - E.g. Google C++ Style Guide
//   - Mention the style guide as a comment on the top of your program code.
//     - // This code follows Google C++ Style Guide.
// - You are allowed to use JSON library to read/write a json file.
//   - E.g. https://nlohmann.github.io/json/.
// - You are allowed to use the std library in C++.
// - Do not use any other libraries, especially to calculate area or intersection.
// - Put appropriate comments for the reviewer to understand your code easily.
// - Submit both your code and your result_convex_hulls.json file.

#include <iomanip>
#include <fstream>

#include "json.hpp"
#include "filter_convex_hulls.h"

using namespace types;
using namespace utils;
using json = nlohmann::json;

// get filtered list of convex hulls that do not 
// overlap more than 50% with other convex hulls
void FilterConvexHulls(std::vector<ConvexHull> convex_hulls, 
                       std::vector<ConvexHull> &filtered_convex_hulls) {

  // for each convex hull, get one as reference
  for (auto& convex_hull_ref : convex_hulls) {

    // total overlap ratio of reference convex hull
    double overlap_ratio = 0;

    // for each convex hull, get one to calculate intersection
    for (auto convex_hull : convex_hulls) {

      // do not calculate intersection of itslef
      if (convex_hull_ref.GetID() != convex_hull.GetID()) {

        // calculate the intersection hull formed by two convex hulls
        ConvexHull intersection_hull;
        if (ConvexHullIntersection(convex_hull_ref,
          convex_hull,
          intersection_hull)) {

          // calculate overlap ratio between reference 
          // convex hull and intersection hull
          double intersection_area = intersection_hull.GetArea();
          double convex_hull_area = convex_hull_ref.GetArea();
          overlap_ratio += intersection_area / convex_hull_area;
        }
      }
    }

    // if total overlap ratio is <= 50%, then push 
    // convex hull info to filtered_convex_hulls
    if (overlap_ratio <= .5) {
      filtered_convex_hulls.push_back(convex_hull_ref);
    }
  }
}

int main() {
  // read input JSON file
  // [1] https://github.com/nlohmann/json#tofrom-streams-eg-files-string-streams
  std::ifstream json_file_in("convex_hulls.json");
  json convex_hulls_json_in;
  json_file_in >> convex_hulls_json_in;

  // convert JSON object to vector of ConvexHulls
  std::vector<ConvexHull> convex_hulls;
  for (const auto convex_hull : convex_hulls_json_in["convex hulls"]) {
    std::vector<Point> vertices;
    for (const auto vertice : convex_hull["apexes"]) {
      vertices.push_back(Point(vertice["x"], vertice["y"]));
    }

    convex_hulls.push_back(ConvexHull(vertices, convex_hull["ID"]));
  }

  // get vector of filtered convex hulls
  std::vector<ConvexHull> filtered_convex_hulls; 
  FilterConvexHulls(convex_hulls, filtered_convex_hulls);

  // convert vector of filter convex hulls to JSON object
  std::vector<json> filtered_convex_hulls_json;
  for (auto convex_hull : filtered_convex_hulls) {
    std::vector<json> apexes;
    for (const auto vertice : convex_hull.GetVertices()) {

      apexes.push_back(json({ {
          "x", vertice.x }, {
          "y", vertice.y }
        }));
    }

    filtered_convex_hulls_json.push_back(json({ {
        "ID", convex_hull.GetID() }, {
        "apexes", apexes }
      }));
  }
  
  // add filtered_convex_hulls_json to "result convex hulls" field
  json convex_hulls_json_out; // output json
  convex_hulls_json_out["result convex hulls"] = filtered_convex_hulls_json;

  // write a results JSON file
  // [1] https://github.com/nlohmann/json#tofrom-streams-eg-files-string-streams
  std::ofstream json_file_out("result_convex_hulls.json");
  json_file_out << std::setw(3) << convex_hulls_json_out << std::endl;
}