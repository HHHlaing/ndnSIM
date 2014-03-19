/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (C) 2014 Named Data Networking Project
 * See COPYING for copyright and distribution information.
 */

#ifndef NFD_FACE_ETHERNET_FACTORY_HPP
#define NFD_FACE_ETHERNET_FACTORY_HPP

#include "ethernet-face.hpp"
#include "protocol-factory.hpp"

namespace nfd {

class NetworkInterfaceInfo;

class EthernetFactory : public ProtocolFactory
{
public:
  /**
   * \brief Exception of EthernetFactory
   */
  struct Error : public ProtocolFactory::Error
  {
    Error(const std::string& what) : ProtocolFactory::Error(what) {}
  };

  // from ProtocolFactory
  virtual void
  createFace(const FaceUri& uri,
             const FaceCreatedCallback& onCreated,
             const FaceConnectFailedCallback& onConnectFailed);

  /**
   * \brief Create an EthernetFace to communicate with the given multicast group
   *
   * If this method is called twice with the same interface and group, only
   * one face will be created. Instead, the second call will just retrieve
   * the existing face.
   *
   * \param interface Local network interface
   * \param address   Ethernet broadcast/multicast destination address
   *
   * \returns always a valid shared pointer to an EthernetFace object,
   *          an exception will be thrown if the creation fails
   *
   * \throws EthernetFactory::Error or EthernetFace::Error
   */
  shared_ptr<EthernetFace>
  createMulticastFace(const shared_ptr<NetworkInterfaceInfo>& interface,
                      const ethernet::Address& address);

private:
  void
  afterFaceFailed(const std::string& interfaceName,
                  const ethernet::Address& address);

  /**
   * \brief Look up EthernetFace using specified interface and address
   *
   * \returns shared pointer to the existing EthernetFace object or
   *          empty shared pointer when such face does not exist
   *
   * \throws never
   */
  shared_ptr<EthernetFace>
  findMulticastFace(const std::string& interfaceName,
                    const ethernet::Address& address) const;

private:
  typedef std::map< std::pair<std::string, ethernet::Address>,
                    shared_ptr<EthernetFace> > MulticastFacesMap;
  MulticastFacesMap m_multicastFaces;
};

} // namespace nfd

#endif // NFD_FACE_ETHERNET_FACTORY_HPP
