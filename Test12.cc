/*
 * Copyright (c) 2011 University of Kansas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Justin Rohrer <rohrej@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  https://resilinets.org/
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

/*
 * This example program allows one to run ns-3 DSDV, AODV, or OLSR under
 * a typical random waypoint mobility model.
 *
 * By default, the simulation runs for 200 simulated seconds, of which
 * the first 50 are used for start-up time.  The number of nodes is 50.
 * Nodes move according to RandomWaypointMobilityModel with a speed of
 * 20 m/s and no pause time within a 300x1500 m region.  The WiFi is
 * in ad hoc mode with a 2 Mb/s rate (802.11b) and a Friis loss model.
 * The transmit power is set to 7.5 dBm.
 *
 * It is possible to change the mobility and density of the network by
 * directly modifying the speed and the number of nodes.  It is also
 * possible to change the characteristics of the network by changing
 * the transmit power (as power increases, the impact of mobility
 * decreases and the effective density increases).
 *
 * By default, OLSR is used, but specifying a value of 2 for the protocol
 * will cause AODV to be used, and specifying a value of 3 will cause
 * DSDV to be used.
 *
 * By default, there are 10 source/sink data pairs sending UDP data
 * at an application rate of 2.048 Kb/s each.    This is typically done
 * at a rate of 4 64-byte packets per second.  Application data is
 * started at a random time between 50 and 51 seconds and continues
 * to the end of the simulation.
 *
 * The program outputs a few items:
 * - packet receptions are notified to stdout such as:
 *   <timestamp> <node-id> received one packet from <src-address>
 * - each second, the data reception statistics are tabulated and output
 *   to a comma-separated value (csv) file
 * - some tracing and flow monitor configuration that used to work is
 *   left commented inline in the program
 */

#include "ns3/aodv-module.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
// #include "ns3/dsdv-module.h"
// #include "ns3/dsr-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
// #include "ns3/olsr-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/netanim-module.h"
#include <fstream>
#include <iostream>
#include "ns3/greyattackaodv-module.h"


using namespace ns3;
// using namespace dsr;

NS_LOG_COMPONENT_DEFINE("manet-routing-compare"); //setting up a structured and manageable logging system within an ns-3 simulation

/**
 * Routing experiment class.
 *
 * It handles the creation and run of an experiment.
 */
class RoutingExperiment
{
  public:
    RoutingExperiment();
    /**
     * Run the experiment.
     */
    void Run();

    /**
     * Handles the command-line parameters.
     * \param argc The argument count.
     * \param argv The argument vector.
     */
    void CommandSetup(int argc, char** argv);

  private:
    /**
     * Setup the receiving socket in a Sink Node.
     * \param addr The address of the node.
     * \param node The node pointer.
     * \return The socket.
     */
    Ptr<Socket> SetupPacketReceive(Ipv4Address addr, Ptr<Node> node);
    /**
     * Receive a packet.
     * \param socket The receiving socket.
     */
    void ReceivePacket(Ptr<Socket> socket);
    /**
     * Compute the throughput.
     */
    void CheckThroughput();

    uint32_t port{9};            //!< Receiving port number.
    uint32_t bytesTotal{0};      //!< Total received bytes.
    uint32_t packetsReceived{0}; //!< Total received packets.

    std::string m_CSVfileName{"manet-routing.output.csv"};        //!< CSV filename.
    // int m_nSinks{5};                                             //!< Number of s`k nodes.      Destination for the data packets
    std::string m_protocolName{"AODV"};                         //!< Protocol name.              
    double m_txp{7};                                           //!< Tx power.                   Transmission Power in dBm
    bool m_traceMobility{true};                               //!< Enable mobility tracing.     Simulation will keep track of the mobility of the nodes
    bool m_flowMonitor{true};       
    
                             //!< Enable FlowMonitor.           provides detailed statistics about network flow, such as the number of transmitted packets, transmission rate

    
    AodvHelper aodv;
    // OlsrHelper olsr;
    // DsdvHelper dsdv;
    // DsrHelper dsr;
    // DsrMainHelper dsrMain;
    Ipv4ListRoutingHelper list;
    greyattackaodvHelper aodv_greyattack;
    InternetStackHelper mInternetStack;   
    InternetStackHelper dInternetStack;                   


    void Install_RoutingProtocol();
    NodeContainer cDefendingNodes;
    NodeContainer cMaliciousNodes;


};

RoutingExperiment::RoutingExperiment()
{
}



/**
 * This methods returns the string which is displayed on the screen
*/
static inline std::string
PrintReceivedPacket(Ptr<Socket> socket, Ptr<Packet> packet, Address senderAddress)
{
    //String is beig sent to a particular object
    std::ostringstream oss;

    oss << Simulator::Now().GetSeconds() << " " << socket->GetNode()->GetId();

    //If the sender address is matching
    if (InetSocketAddress::IsMatchingType(senderAddress))
    {
        InetSocketAddress addr = InetSocketAddress::ConvertFrom(senderAddress);
        oss << " received one packet from " << addr.GetIpv4();
    }
    //If we don't know the IP address, then just print this
    else
    {
        oss << " received one packet!";
    }
    return oss.str();
}

/**
 * Crucial for tracking and analyzing packet reception within the network simulator
 * 
 * Packet Reception - Listens for incoming packets
 * Data Accumelation - Increments the total number of bytes
 * Packet Count - Increments the packetreceived which is a criteria performance matrics in network simulator
 * Logging - Each packet received is logged using "NS_LOG_UNCOND" (Timestamp, NodeID, SendersAddress)
*/
void
RoutingExperiment::ReceivePacket(Ptr<Socket> socket)
{
    Ptr<Packet> packet;
    Address senderAddress;
    while ((packet = socket->RecvFrom(senderAddress))) //
    {
        bytesTotal += packet->GetSize(); //We will be incrementing the packets 
        packetsReceived += 1; //Each packet will be monitored
        NS_LOG_UNCOND(PrintReceivedPacket(socket, packet, senderAddress));
    }
}

void
RoutingExperiment::Install_RoutingProtocol()
{
    // set up AODV protocol helpers

    aodv_greyattack.Set("PercentDrop", DoubleValue(0.6)); // Hypothetical attribute to enable attack behavior
    aodv_greyattack.Set("mStrat", UintegerValue(1)); // Hypothetical attribute to set the probability of dropping a packet


    mInternetStack.SetRoutingHelper(aodv_greyattack);
    dInternetStack.SetRoutingHelper(aodv);

    // install it
    mInternetStack.Install(cMaliciousNodes);
    dInternetStack.Install(cDefendingNodes);

    
    NS_LOG_UNCOND("Done Routing Protocol");

    return;
}

/**
 * Crucial role in periodically measuring and logging the network's throughput performance
 * 
 * Throughput Calculation - Calculated the Throughput in kbps this is done by diving by the number of intervals (ie. 1sec)
 * Reseting Counter - It resets the "byteTotal" and "packetReceived", ensures that each thoughput calculates the proceding interval
 * Logging to CSV - (NumberOfPacketsReceived,NumberOfSinkNodes, ProtocolName, TransmissionPower)
 * SchedulingNextCheck - Calls itself after every 1 second
*/
void
RoutingExperiment::CheckThroughput()
{
    //Throughput Calculation
    double kbs = (bytesTotal * 8.0) / 1000;
    //Reseting Counter
    bytesTotal = 0;

    std::ofstream out(m_CSVfileName, std::ios::app);

    //This is not being used
    // //Logging to CSV
    // out << (Simulator::Now()).GetSeconds() << "," << kbs << "," << packetsReceived << ","
    //     << m_nSinks << "," << m_protocolName << "," << m_txp << "" << std::endl;

    out.close();
    //Reseting Counter
    packetsReceived = 0;

    //SchedulingNextCheck
    Simulator::Schedule(Seconds(1.0), &RoutingExperiment::CheckThroughput, this);
}

/**
 * Configure a network node to receive packets
 * 
 * Socket Type Identification - Creates the suitable socket
 * Socket Creation - Socket is made on the specific node using the socket factory
 * Binding Socket - Socket is then bounded to a IP Address and Port forming a local endpoint for data reception
 * Reception Callback - Sets a callback function 'ReceivePacket' where ever the socket receives a packet, Responsible for handling incoming packets
*/
Ptr<Socket>
RoutingExperiment::SetupPacketReceive(Ipv4Address addr, Ptr<Node> node)
{
    //Socket Type Identification 
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    //Socket Creation
    Ptr<Socket> sink = Socket::CreateSocket(node, tid);
    InetSocketAddress local = InetSocketAddress(addr, port);
    //Binding Socket
    sink->Bind(local);
    //Reception Callback
    sink->SetRecvCallback(MakeCallback(&RoutingExperiment::ReceivePacket, this)); 

    return sink;
}

/**
 * Parse and handle command-line arguments 
 * 
 * Initialize Command Line Parsing - creating a CommandLine object, which is used to specify which command-line arguments the program can accept. This object is initialized with __FILE__
 * Define Accepted Arguments - 
 * Parse Command-Line Arguments
 * Check Protocol Validity
*/
void
RoutingExperiment::CommandSetup(int argc, char** argv)
{
    CommandLine cmd(__FILE__);
    cmd.AddValue("CSVfileName", "The name of the CSV output file name", m_CSVfileName);
    cmd.AddValue("traceMobility", "Enable mobility tracing", m_traceMobility);
    cmd.AddValue("protocol", "Routing protocol (AODV)", m_protocolName);
    // cmd.AddValue("protocol", "Routing protocol (OLSR, AODV, DSDV, DSR)", m_protocolName);
    cmd.AddValue("flowMonitor", "enable FlowMonitor", m_flowMonitor);
    cmd.Parse(argc, argv);

    // std::vector<std::string> allowedProtocols{"OLSR", "AODV", "DSDV", "DSR"};
    std::vector<std::string> allowedProtocols{"AODV"};


    if (std::find(std::begin(allowedProtocols), std::end(allowedProtocols), m_protocolName) ==
        std::end(allowedProtocols))
    {
        NS_FATAL_ERROR("No such protocol:" << m_protocolName);
    }
}



//First level class is over. RoutingExperiment
int
main(int argc, char* argv[])
{
    //Print out the errors by using custom error code
    LogComponentEnable("greyattackaodvRoutingProtocol",LOG_LEVEL_INFO);
    RoutingExperiment experiment; //Object is created
    experiment.CommandSetup(argc, argv);
    experiment.Run();
    
    //LogComponentEnable ("AnimationInterface", LOG_LEVEL_ALL);
    return 0;
}

void
RoutingExperiment::Run()
{
    Packet::EnablePrinting();

    // blank out the last output file and write the column headers
    std::ofstream out(m_CSVfileName);
    out << "SimulationSecond,"
        << "ReceiveRate,"
        << "PacketsReceived,"
        << "NumberOfSinks,"
        << "RoutingProtocol,"
        << "TransmissionPower" << std::endl;
    out.close();

    int ndefendingWifis = 21;
    int nattackingWifis = 3;

    double TotalTime = 200.0;
    std::string rate("2048bps"); //Changing this can simulate different network conditions
    std::string phyMode("DsssRate11Mbps");
    std::string tr_name("manet-routing-compare"); //No Extension needed, it will be attached at the end
    int nodeSpeed = 10; // in m/s
    int nodePause = 0;  // in s

    Config::SetDefault("ns3::OnOffApplication::PacketSize", StringValue("800"));
    Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue(rate));

    // Set Non-unicastMode rate to unicast mode
    Config::SetDefault("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue(phyMode));

    //Creates and manages groups of nodes
    // NodeContainer cMaliciousNodes;
    cMaliciousNodes.Create(nattackingWifis);
    cDefendingNodes.Create(ndefendingWifis);

    // setting up wifi phy and channel using helpers
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211b);

    YansWifiPhyHelper wifiPhy;
    YansWifiChannelHelper wifiChannel;
    wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
    wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel");
    wifiPhy.SetChannel(wifiChannel.Create());

    // Add a mac and disable rate control
    WifiMacHelper wifiMac;
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                 "DataMode",
                                 StringValue(phyMode),
                                 "ControlMode",
                                 StringValue(phyMode));

    wifiPhy.Set("TxPowerStart", DoubleValue(m_txp));
    wifiPhy.Set("TxPowerEnd", DoubleValue(m_txp));

    wifiMac.SetType("ns3::AdhocWifiMac");
    // NetDeviceContainer adhocDevices = wifi.Install(wifiPhy, wifiMac, cMaliciousNodes);

    NetDeviceContainer cMaliciousDevices = wifi.Install(wifiPhy, wifiMac, cMaliciousNodes);
    NetDeviceContainer cDefendingDevices = wifi.Install(wifiPhy, wifiMac, cDefendingNodes);



    // Enable Pcap output for the Malicious Nodes
    wifiPhy.EnablePcap("wifi-simulation-malicious", cMaliciousNodes);

    //Enable Pcap output for the Benign Nodes
    wifiPhy.EnablePcap("wifi-simulation-benign", cDefendingNodes);



    MobilityHelper mobilityAdhoc;
    int64_t streamIndex = 0; // used to get consistent mobility across scenarios

    ObjectFactory pos;
    pos.SetTypeId("ns3::RandomRectanglePositionAllocator");
    // pos.Set("X", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=300.0]"));
    // pos.Set("Y", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=1500.0]"));

    pos.Set("X", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=100.0]")); // Adjusted area size
    pos.Set("Y", StringValue("ns3::UniformRandomVariable[Min=0.0|Max=100.0]")); // Adjusted area size

    Ptr<PositionAllocator> taPositionAlloc = pos.Create()->GetObject<PositionAllocator>();
    streamIndex += taPositionAlloc->AssignStreams(streamIndex);

    std::stringstream ssSpeed;
    ssSpeed << "ns3::UniformRandomVariable[Min=0.0|Max=" << nodeSpeed << "]";
    std::stringstream ssPause;
    ssPause << "ns3::ConstantRandomVariable[Constant=" << nodePause << "]";
    mobilityAdhoc.SetMobilityModel("ns3::RandomWaypointMobilityModel",
                                   "Speed",
                                   StringValue(ssSpeed.str()),
                                   "Pause",
                                   StringValue(ssPause.str()),
                                   "PositionAllocator",
                                   PointerValue(taPositionAlloc));
    mobilityAdhoc.SetPositionAllocator(taPositionAlloc);
    mobilityAdhoc.Install(cMaliciousNodes); 
    mobilityAdhoc.Install(cDefendingNodes);


    // Create a position allocator
    // Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    // // Add positions for each UAV
    //     // positionAlloc->Add(Vector(100, 200, 30));  // UAV 1 position
    //     // positionAlloc->Add(Vector(150, 250, 60));  // UAV 2 position
    //     // positionAlloc->Add(Vector(200, 100, 90));  // UAV 3 position
    //     // positionAlloc->Add(Vector(250, 150, 120)); // UAV 4 position
    //     // positionAlloc->Add(Vector(50, 50, 150));   // UAV 5 position
    //     // positionAlloc->Add(Vector(300, 200, 180)); // UAV 6 position
    //     // positionAlloc->Add(Vector(100, 300, 210)); // UAV 7 position
    //     // positionAlloc->Add(Vector(200, 50, 240));  // UAV 8 position
    //     // positionAlloc->Add(Vector(250, 300, 270)); // UAV 9 position
    //     // positionAlloc->Add(Vector(50, 250, 300));  // UAV 10 position

    //     positionAlloc->Add(Vector(0, 10, 10));
    //     positionAlloc->Add(Vector(0, 20, 20));
    //     positionAlloc->Add(Vector(0, 30, 30));
    //     positionAlloc->Add(Vector(0, 40, 40));
    //     positionAlloc->Add(Vector(0, 50, 50));
    //     positionAlloc->Add(Vector(0, 60, 60));
    //     positionAlloc->Add(Vector(0, 70, 70));
    //     positionAlloc->Add(Vector(0, 80, 80));
    //     positionAlloc->Add(Vector(0, 90, 90));
    //     positionAlloc->Add(Vector(0, 100, 100));






    // // mobilityAdhoc.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    // mobilityAdhoc.SetPositionAllocator(positionAlloc);
    // mobilityAdhoc.Install(cMaliciousNodes);

    streamIndex += mobilityAdhoc.AssignStreams(cMaliciousNodes, streamIndex);


    



    // if (m_protocolName == "OLSR")
    // {
    //     list.Add(olsr, 100);
    //     internet.SetRoutingHelper(list);
    //     internet.Install(cMaliciousNodes);
    // }
    if (m_protocolName == "AODV")
    {
        // list.Add(aodv, 100);
        // internet.SetRoutingHelper(list);
        // internet.SetRoutingHelper(greyattackaodv); // This will set greyattackaodv as the routing protocol
        // internet.Install(cMaliciousNodes);
        Install_RoutingProtocol();
    }
    // else if (m_protocolName == "DSDV")
    // {
    //     list.Add(dsdv, 100);
    //     internet.SetRoutingHelper(list);
    //     internet.Install(cMaliciousNodes);
    // }
    // else if (m_protocolName == "DSR")
    // {
    //     internet.Install(cMaliciousNodes);
    //     dsrMain.Install(dsr, cMaliciousNodes);
    //     if (m_flowMonitor)
    //     {
    //         NS_FATAL_ERROR("Error: FlowMonitor does not work with DSR. Terminating.");
    //     }
    // }
    else
    {
        NS_FATAL_ERROR("No such protocol:" << m_protocolName);
    }

    NS_LOG_INFO("assigning ip address");

    Ipv4AddressHelper addressAdhoc;
    addressAdhoc.SetBase("192.168.0.0", "255.255.255.0");
    Ipv4InterfaceContainer adhocDefendingInterfaces;
    Ipv4InterfaceContainer adhocMaliciousInterfaces;
    adhocDefendingInterfaces = addressAdhoc.Assign(cDefendingDevices);
    adhocMaliciousInterfaces = addressAdhoc.Assign(cMaliciousDevices);


    OnOffHelper onoff1("ns3::UdpSocketFactory", Address());
    onoff1.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1.0]"));
    onoff1.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0.0]"));


    // //This is 5 + 5 Attactaing and defending nodes
    // for (int i = 0; i < m_nSinks; i++)
    // {
    //     Ptr<Socket> sink = SetupPacketReceive(adhocInterfaces.GetAddress(i), cMaliciousNodes.Get(i));
        

    //     AddressValue remoteAddress(InetSocketAddress(adhocInterfaces.GetAddress(i), port));
    //     onoff1.SetAttribute("Remote", remoteAddress);

    //     Ptr<UniformRandomVariable> var = CreateObject<UniformRandomVariable>();
    //     ApplicationContainer temp = onoff1.Install(cMaliciousNodes.Get(i + m_nSinks));
    //     //This is the start time for the packet sending
    //     temp.Start(Seconds(var->GetValue(5.0, 10.0)));
    //     temp.Stop(Seconds(TotalTime));
    // }

        // Ptr<Socket> sink = SetupPacketReceive(adhocInterfaces.GetAddress(0), cDefendingNodes.Get(1));

        //Defending nodes will be receiving
        Ptr<Socket> sink = SetupPacketReceive(adhocDefendingInterfaces.GetAddress(1), cDefendingNodes.Get(1));

        

        AddressValue remoteAddress(InetSocketAddress(adhocDefendingInterfaces.GetAddress(1), port));
        onoff1.SetAttribute("Remote", remoteAddress);

        Ptr<UniformRandomVariable> var = CreateObject<UniformRandomVariable>();
        ApplicationContainer temp = onoff1.Install(cDefendingNodes.Get(0));
        //This is the start time for the packet sending
        temp.Start(Seconds(var->GetValue(5.0, 10.0)));
        temp.Stop(Seconds(TotalTime));

    // std::stringstream ss;
    // ss << ndefendingWifis;
    // std::string nodes = ss.str();

    // std::stringstream ss2;
    // ss2 << nodeSpeed;
    // std::string sNodeSpeed = ss2.str();

    // std::stringstream ss3;
    // ss3 << nodePause;
    // std::string sNodePause = ss3.str();

    // std::stringstream ss4;
    // ss4 << rate;
    // std::string sRate = ss4.str();

    
    


    // NS_LOG_INFO("Configure Tracing.");
    // tr_name = tr_name + "_" + m_protocolName +"_" + nodes + "nodes_" + sNodeSpeed + "speed_" +
    // sNodePause + "pause_" + sRate + "rate";

    // AsciiTraceHelper ascii;
    // Ptr<OutputStreamWrapper> osw = ascii.CreateFileStream(tr_name + ".tr");
    // wifiPhy.EnableAsciiAll(osw);
    AsciiTraceHelper ascii;
    MobilityHelper::EnableAsciiAll(ascii.CreateFileStream(tr_name + ".mob"));

    FlowMonitorHelper flowmonHelper;
    Ptr<FlowMonitor> flowmon;
    if (m_flowMonitor)
    {
        flowmon = flowmonHelper.InstallAll();
    }

    NS_LOG_INFO("Run Simulation.");

    CheckThroughput();

    // After setting up your simulation, before starting it
    AnimationInterface anim("manet-routing-compare.xml"); // This line enables NetAnim for your simulation

    //This set of code is responsible for changing the colour of the Malicipus nodes in NetAnim
    for (uint32_t i = 0; i < cDefendingNodes.GetN(); ++i) {
        anim.UpdateNodeColor(cDefendingNodes.Get(i)->GetId(), 0, 255, 0); // Update color to green for each node
    }


    //Code added to get the trace file
    anim.EnableIpv4RouteTracking ((tr_name + "route-track.xml").c_str(), Seconds(0), Seconds(TotalTime), Seconds(0.1));
    anim.EnablePacketMetadata(true);
    anim.SetMaxPktsPerTraceFile(1200000);

    Simulator::Stop(Seconds(TotalTime));
    Simulator::Run();

    if (m_flowMonitor)
    {
        flowmon->SerializeToXmlFile(tr_name + ".flowmon", false, false);
    }

    Simulator::Destroy();
}
