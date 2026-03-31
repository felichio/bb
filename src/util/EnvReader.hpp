#pragma once

#include <string>
#include <unordered_map>

namespace bb
{
  class EnvReader
  {
    public:
      
      void populateEnv();
      std::string getKey(const std::string &key);
      static EnvReader *getReader();
    private:
      EnvReader(const std::string &path);
      static EnvReader *ref;
      std::unordered_map<std::string, std::string> m_envmap;
  };
}