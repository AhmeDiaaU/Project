#include "Virtual_Disk.h"
using namespace std;

// Initialize the static file stream object for the virtual disk
fstream Virtual_Disk::Disk;

// Constants
const int CLUSTER_SIZE = 1024; // Define cluster size as a constant for clarity

// Functions

// Create or open the virtual disk file
void Virtual_Disk::createOrOpenDisk(const string& path) {
    // Attempt to open the file in read/write binary mode
    Disk.open(path, ios::in | ios::out | ios::binary);

    // If the file doesn't exist, create it
    if (!Disk.is_open()) {
        Disk.open(path, ios::in | ios::out | ios::binary | ios::trunc);
    }

    // Ensure the file is open
    if (!Disk.is_open()) {
        throw runtime_error("Failed to create or open the virtual disk file.");
    }
}

// Write a cluster of data to the virtual disk
void Virtual_Disk::writeCluster(const vector<char>& cluster, int clusterIndex) {
    // Validate cluster size
    if (cluster.size() != CLUSTER_SIZE) {
        throw invalid_argument("Cluster size must be exactly 1024 bytes.");
    }

    // Move the write pointer to the specified cluster position
    Disk.seekp(clusterIndex * CLUSTER_SIZE, ios::beg);

    // Write the cluster data to the disk
    Disk.write(cluster.data(), CLUSTER_SIZE);

    // Check if the write operation failed
    if (!Disk) {
        throw runtime_error("Failed to write cluster to the virtual disk.");
    }

    // Flush the stream to ensure data is written to disk
    Disk.flush();
}

// Read a cluster of data from the virtual disk
vector<char> Virtual_Disk::readCluster(int clusterIndex) {
    // Move the read pointer to the specified cluster position
    Disk.seekg(clusterIndex * CLUSTER_SIZE, ios::beg);

    // Create a vector to hold the cluster data
    vector<char> cluster(CLUSTER_SIZE);

    // Read the cluster data from the disk
    Disk.read(cluster.data(), CLUSTER_SIZE);

    // Check if the read operation failed
    if (!Disk) {
        throw runtime_error("Failed to read cluster from the virtual disk.");
    }

    return cluster;
}

// Check if the virtual disk is new (empty)
bool Virtual_Disk::isNew() {
    // Move the file pointer to the end of the file
    Disk.seekg(0, ios::end);

    // Get the file size
    int fileSize = static_cast<int>(Disk.tellg());

    // Return true if the file is empty
    return (fileSize == 0);
}

// Close the virtual disk file
void Virtual_Disk::closeDisk() {
    if (Disk.is_open()) {
        Disk.close();
    }
}