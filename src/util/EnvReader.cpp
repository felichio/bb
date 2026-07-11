#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <util/EnvReader.hpp>

namespace bb 
{
  // define ref
  EnvReader* EnvReader::ref = nullptr;

  EnvReader *EnvReader::getReader()
  {
    if (EnvReader::ref == nullptr)
    {
      const std::filesystem::path bin_dir = std::filesystem::canonical("/proc/self/exe").parent_path();
      const std::filesystem::path env_path = bin_dir / ".." / "config" / ".env";
      ref = new EnvReader(env_path);
    }
    return ref;
  }

  EnvReader::EnvReader(const std::string &path)
  {
    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
      spdlog::warn("Env file at " + path + " not opened!");
    }
    else
    {
      std::string line;
      while(std::getline(ifs, line))
      {
        std::size_t index = line.find("=");
        std::string key = line.substr(0, index);
        std::string value = line.substr(index + 1);
        // store to map
        m_envmap[key] = value;
      }
    }
    populateEnv();
  }

  void EnvReader::populateEnv()
  {
    for (std::unordered_map<std::string, std::string>::const_iterator it = m_envmap.begin(); it != m_envmap.end(); ++it)
    {
      // overwrite 0
      setenv(it->first.c_str(), it->second.c_str(), 0);
    }
  }

  std::string EnvReader::getKey(const std::string &key)
  {
    char *value = getenv(key.c_str());
    std::string result = "";
    if (value != NULL)
    {
      result = std::string(value);
    }
    return result;
  }
}