/**
 * \file drmhelper.h
 *
 * This file provides static to access common functions provided by the
 * DRM such as getting a map of the available channels.
 *
 */

#ifndef DRMHELPER_H
#define DRMHELPER_H

#include "boost/date_time/posix_time/posix_time.hpp"
#include "sclhelper.hpp"
#include "drm.pb.h"

typedef scl_drm::channelSpec ChannelSpec;
typedef std::vector<scl_drm::channelSpec> ChannelMap;
typedef std::vector<scl_drm::channelSpec>::iterator ChannelMapIt;

//! Exception which can be thrown by DRM helper class
class DrmHelperException : public std::exception
{
private:
    std::string d_message;
public:
    DrmHelperException(const std::string &message) throw()
        :exception(), d_message(message)
    {};
    virtual const char* what() const throw()
    {
        return d_message.c_str();
    };
    virtual ~DrmHelperException() throw()
    {};
};

class DrmHelper
{
    public:
        static ChannelMap getChannelMap(void)
        {
            GateFactory &myFactory = GateFactory::getInstance();
            sclGate *controlGate = myFactory.createGate("drmgenerator", "control");
            
            scl_drm::control request, reply;

            // create control packet to request channel specification
            cout << "Send GET_CHANNEL_LIST request" << endl;
            request.set_type(scl_drm::REQUEST);
            request.set_command(scl_drm::GET_CHANNEL_LIST);
            controlGate->sendProto(request);

            // wait for answer from DRM
            cout << "Wait for reply .." << endl;
            controlGate->recvProto(reply);
            assert(reply.type() == scl_drm::REPLY);
            cout << "Got reply:" << endl;
            cout << "Number of channels is: " << reply.channelmap_size() << endl;
            
            // convert DRM reply to channelmap object
            ChannelMap drmChannelMap;
            for (int i = 0; i < reply.channelmap_size(); i++) {
                drmChannelMap.push_back(reply.channelmap(i));
            }
            return drmChannelMap;
        };

        static ChannelSpec getChannel(const scl_drm::channelProp property)
        {
            GateFactory &myFactory = GateFactory::getInstance();
            sclGate *controlGate = myFactory.createGate("drmgenerator", "control");

            scl_drm::control request, reply;

            // create control packet to request channel specification
            cout << "Send GET_SINGLE_CHANNEL request" << endl;
            request.set_type(scl_drm::REQUEST);
            request.set_command(scl_drm::GET_SINGLE_CHANNEL);
            request.set_prop(property);
            controlGate->sendProto(request);

            // wait for answer from DRM
            cout << "Wait for reply .." << endl;
            controlGate->recvProto(reply);
            assert(reply.type() == scl_drm::REPLY);
            cout << "Got reply:" << endl;
            if (reply.channelmap_size() == 0)
                throw DrmHelperException("DRM did not send a proper channel configuration.");
            return reply.channelmap(0);
        };
        
        static uint64_t getTimeSinceEpoch()
        {
            boost::posix_time::ptime epoch = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
            boost::posix_time::ptime other = boost::posix_time::microsec_clock::local_time();
            return (other-epoch).total_milliseconds();
        }
};

#endif // DRMHELPER_H