#ifndef Geometry_h
#define Geometry_h

#include "TestbeamReco/interface/NTupleReader.h"
#include "TestbeamReco/interface/Utility.h"

class DefaultGeometry
{
public:
    DefaultGeometry(const int v=0) : voltage(v){}
    virtual ~DefaultGeometry() = default;
    const int voltage;
    class VoltageDependence
    {
    public:
        double noiseAmpThreshold;
        double signalAmpThreshold;
    };

    std::map<int, std::vector<int>> indexToGeometryMap = {{0,{0,0}}};
    std::vector<std::vector<int>> geometry = {{0}};
    std::map<int, bool> acLGADChannelMap = {{0,false}};
    std::map<int, double> amplitudeCorrectionFactor = {{0,1.0}};
    std::map<int, double> timeCalibrationCorrection = {{0,0.0}};
    double stripWidth = 0.08;
    double pitch = 999;
    double sensorCenter = 0;
    double sensorCenterY = 0;
    std::vector<double> stripCenterXPosition = {{0,0.0}};
    std::vector<double> stripCenterYPosition = {{0,0.0}};
    int numLGADchannels = 0;
    int extraChannelIndex = -1;
    int photekIndex = 7;
    int lowGoodStripIndex = 1;
    int highGoodStripIndex = 5;
    double alpha = 0.0;
    double beta  = 0.0;
    double gamma =  0.0;
    double z_dut = 0.0;
    double xBinSize = 0.05;
    double yBinSize = 0.05;
    double xBinSize_delay_corr = 0.05;
    double yBinSize_delay_corr = 0.2;
    double xmin =  -1;
    double xmax =  -1;
    double ymin =  -1;
    double ymax =  -1;
    double positionRecoMaxPoint = 1.0;
    double positionRecoMaxPointRow = 1.0;
    double positionRecoMaxPointCol = 1.0;
    double photekSignalThreshold = 200.0; //in mV
    double photekSignalMax = 350.0; //in mV
    double noiseAmpThreshold = 15.0; //in mV
    double signalAmpThreshold = 15.0; //in mV
    bool isPadSensor = false;
    bool isHPKStrips = false;
    bool enablePositionReconstruction = false;
    bool enablePositionReconstructionPad = false;
    bool uses2022Pix = false;
    bool usesMay2023Tracker = false;
    bool isHorizontal = false;
    int minPixHits = 0;
    int minStripHits = 7;
    int CFD_threshold = 20;
    std::vector<std::string> CFD_list = {"5","10","15","20","25","30","35","40","50","60"};
    std::vector<double> positionRecoPar = {-1};
    std::vector<double> positionRecoParRow = {-1};
    std::vector<double> positionRecoParCol = {-1};
    std::vector<double> positionRecoParRight = {-1};
    std::vector<double> positionRecoParLeft = {-1};
    std::vector<double> positionRecoParTop = {-1};
    std::vector<double> positionRecoParBot = {-1};
    std::vector<std::vector<double>> sensorEdges = {{-999.9, -999.9}, {999.9, 999.9}};
    std::vector<std::vector<double>> sensorEdgesTight = {{-999.9, -999.9}, {999.9, 999.9}};
    std::vector<std::vector<double>> sensorEdgesExtra = {{-999.9, -999.9}, {999.9, 999.9}};
    std::vector<std::vector<double>> ySlices = {{-999.9, 999.9}, {-999.9, 999.9}};
    std::vector<std::vector<double>> xSlices = {{-999.9, 999.9}, {-999.9, 999.9}};
    std::vector<std::vector<double>> boxes_XY = {{-999.9, -999.9,-999.9, -999.9}};
    std::vector<utility::ROI> regionsOfIntrest = {{"default", -999.9,999.9, -999.9,999.9}};
};

class ETL_Test_100V_1_Geometry: public DefaultGeometry
// ETL_Test_100V_1
{
public:
    // 
    // Used lecroy scope channels 0-7
    // Scope channel 0-6 was AC channels, and scope channel 7 was the photek
    // 
    // |-----------|             -----
    // | 0 0 0 0 0 |             |777|
    // | 1 1 1 1 1 |             |777|
    // | 2 2 2 2 2 |             -----
    // | 3 3 3 3 3 |
    // | 4 4 4 4 4 |
    // | 5 5 5 5 5 |
    // | 6 6 6 6 6 |
    // |-----------|

    ETL_Test_100V_1_Geometry(const int v=0) : voltage(v){}
    const int voltage;
    std::map<int, std::vector<int>> indexToGeometryMap = {{0,{0,0}}, {1,{0,1}}, {2,{0,2}}, {3,{1,0}}};
    std::vector<std::vector<int>> geometry = {{0,1,2}, {3}};
    std::map<int, bool> acLGADChannelMap = {{0,true}, {1,true}, {2,true}, {3,false}};
    std::map<int, double> amplitudeCorrectionFactor = {{0,1.0}, {1,1.0}, {2,1.0}, {3,1.0}};
    std::map<int, double> timeCalibrationCorrection = {{0,1.0}, {1,1.0}, {2,1.0}, {3,1.0}};
    double sensorCenter  = 0.45; // Lab-Tracker's frame ->  y_dut
    double sensorCenterY = 1.50; // Lab-Tracker's frame -> -x_dut
    std::vector<double> stripCenterXPosition = {1.492, 0.998, 0.491, -0.010};
    int numLGADchannels = 7;
    int photekIndex = 1;
    double alpha = -0.29;
    double beta  =  0.00;
    double gamma =  0.00;
    double z_dut = -0.75;
    double xBinSize = 0.15;
    double yBinSize = 0.15;
    double xmin = -10.00; // Sensor's local frame
    double xmax =  10.00; // Sensor's local frame
    double ymin =  -6.00; // Sensor's local frame
    double ymax =   6.00; // Sensor's local frame
    double positionRecoMaxPoint = 1.0; // 0.71;
    double photekSignalThreshold = 100.0; //in mV
    double photekSignalMax = 1000.0; //in mV
    double noiseAmpThreshold  = 0.0;
    double signalAmpThreshold = 0.0;
    int CFD_threshold = 50;
    std::vector<double> positionRecoPar = {0.250000, -1.057035, -2.963484, 58.283739, -422.846674, 923.928427};
    std::vector<std::vector<double>> sensorEdges = {{-1.8, -4.70}, {1.8, 4.70}}; // Sensor's local frame
    std::vector<std::vector<double>> sensorEdgesTight = {{stripCenterXPosition[highGoodStripIndex], -4.6}, {stripCenterXPosition[lowGoodStripIndex], 4.6}}; // Sensor's local frame
};

#endif
