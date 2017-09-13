// Copyright (c) 2017-2017 Coburn Wightman
// SPDX-License-Identifier: BSL-1.0

#include <SoapySDR/ConverterRegistry.hpp>
#include <iostream>

SoapySDR::ConverterRegistry::FormatConverters SoapySDR::ConverterRegistry::formatConverters;

SoapySDR::ConverterRegistry::ConverterRegistry(const std::string &sourceFormat, const std::string &targetFormat, FunctionPriority &priority, ConverterFunction converterFunction)
{
  if (formatConverters[sourceFormat][targetFormat].count(priority) != 0)
    {
      throw std::invalid_argument("conversion already registered: " + sourceFormat + " to " + targetFormat + " priority " + std::to_string(priority) + ".");
    }
  
  formatConverters[sourceFormat][targetFormat][priority] = converterFunction;

  _sourceFormat = sourceFormat;
  _targetFormat = targetFormat;
  _priority = priority;
  _isRegistered = true;

  std::cout << '+';
  return;
}

SoapySDR::ConverterRegistry::ConverterRegistry(void)
{
  _isRegistered = false;
  return;
}

SoapySDR::ConverterRegistry::~ConverterRegistry(void)
{
  return;
}

void SoapySDR::ConverterRegistry::remove(void)
{
  if (_isRegistered == true)
    formatConverters[_sourceFormat][_targetFormat].erase(_priority);

  std::cout << "~";
  return;
}

std::vector<std::string> SoapySDR::ConverterRegistry::listTargetFormats(const std::string &sourceFormat)
{
  std::vector<std::string> targets;

  if (formatConverters.count(sourceFormat) == 0)
    return targets;

  for(const auto &it:formatConverters[sourceFormat])
    {
      std::string targetFormat = it.first;
      targets.push_back(targetFormat);
    }
  
  return targets;
}

std::vector<std::string> SoapySDR::ConverterRegistry::listSourceFormats(const std::string &targetFormat)
{
  std::vector<std::string> sources;

  for(const auto &it:formatConverters)
    {
      std::string sourceFormat = it.first;
      if (formatConverters[sourceFormat].count(targetFormat) > 0)
	sources.push_back(sourceFormat);
    }
  
  return sources;
}

std::vector<SoapySDR::ConverterRegistry::FunctionPriority> SoapySDR::ConverterRegistry::listPriorities(const std::string &sourceFormat, const std::string &targetFormat)
{
  std::vector<FunctionPriority> priorities;
  
  if (formatConverters.count(sourceFormat) == 0)
    throw std::invalid_argument("unsupported source format: " + sourceFormat + "."); 
  
  if (formatConverters[sourceFormat].count(targetFormat) == 0)
    throw std::invalid_argument("unsupported target format: " + targetFormat + ".");

  if (formatConverters[sourceFormat][targetFormat].size() == 0)
    throw std::invalid_argument("no registered functions for: " + sourceFormat + " to "+ targetFormat + ".");

  for(const auto &it:formatConverters[sourceFormat][targetFormat])
    {
      FunctionPriority priority = it.first;
      priorities.push_back(priority);
    }
  
  return priorities;
  
}

SoapySDR::ConverterRegistry::ConverterFunction SoapySDR::ConverterRegistry::getFunction(const std::string &sourceFormat, const std::string &targetFormat)
{
  if (formatConverters.count(sourceFormat) == 0)
    throw std::invalid_argument("unsupported source format: " + sourceFormat + "."); 
  
  if (formatConverters[sourceFormat].count(targetFormat) == 0)
    throw std::invalid_argument("unsupported target format: " + targetFormat + "."); 

  return formatConverters[sourceFormat][targetFormat].rbegin()->second;
}

SoapySDR::ConverterRegistry::ConverterFunction SoapySDR::ConverterRegistry::getFunction(const std::string &sourceFormat, const std::string &targetFormat, const FunctionPriority &priority)
{
  if (formatConverters.count(sourceFormat) == 0)
    throw std::invalid_argument("unsupported source format: " + sourceFormat + "."); 
  
  if (formatConverters[sourceFormat].count(targetFormat) == 0)
    throw std::invalid_argument("unsupported target format: " + targetFormat + "."); 

  if (formatConverters[sourceFormat][targetFormat].count(priority) == 0)
    throw std::invalid_argument("unsupported target priority: " + std::to_string(priority) + "."); 

  return formatConverters[sourceFormat][targetFormat][priority];
}

