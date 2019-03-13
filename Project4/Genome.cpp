#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    string m_name;
    int m_length;
    string m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
    m_name=nm;
    m_sequence=sequence;
    m_length=sequence.size();
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
    string line;
    bool lookForNewName;
    string name = "";
    string sequence = "";
    
    // Make sure first line is a properly formatted name line
    if(!getline(genomeSource, line)) {
        cout << "FAIL";
        return false;
        
    }
    if(line[0] != '>' || line.size() <= 1)
        return false;
    name = line.substr(1);
    lookForNewName = false;
    
    while (getline(genomeSource, line)){
        if(line.size()==0)  // Handle empty lines
            return false;
        // If we come across a > indicating a name line and the previous line was not a name line
        if(line[0] != '>' && lookForNewName) {
            genomes.push_back(Genome(name,sequence));   // Push the previously read genome to the vector
            
            if(line.size() <= 1)    // If the line contains only the >
                return false;
            
            name = line.substr(1);  // Save the name without the >
            
            lookForNewName = false; // indicate the last line was a name - the next can't be a name line
        }
        else {
            // For each letter in the line, if it is not an ACTG or N, return false
            for(int i=0; i<line.size(); i++){
                if(toupper(line[i]) != 'A' || toupper(line[i]) != 'C' || toupper(line[i]) != 'T' || toupper(line[i]) != 'G' || toupper(line[i]) != 'N')
                    return false;
                
                sequence += toupper(line[i]);   // Add current character to sequence
            }
            lookForNewName = true;  // It is now safe to look for a name line after a sequence was read
        }
    }
    
    if(!lookForNewName) // If there was no sequence after the last line if the last line was a name
        return false;
    
    genomes.push_back(Genome(name,sequence));   // Push the last genome and return true
    return true;
}

int GenomeImpl::length() const
{
    return m_length;
}

string GenomeImpl::name() const
{
    return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
    if(position < 0  || length < 0 || position + length > m_sequence.size())
        return false;
    fragment = m_sequence.substr(position, length);
    return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}



