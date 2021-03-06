#include "provided.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include "Trie.h"


class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    // For our Trie to store, stores index to the genome in the genome vector
    struct GenomeSegment {
        int index;
        int position;
    };
    Trie<GenomeSegment> m_trie;
    vector<Genome> m_genomes;
    int m_minimum;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
    m_minimum = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    m_genomes.push_back(genome);
    if(genome.length() >= m_minimum){
        int i=0;
        // for every full segment
        while(i + m_minimum <= genome.length()){
            string fragment;
            genome.extract(i, m_minimum, fragment); // Get subset
            GenomeSegment seg;
            seg.index = (int) m_genomes.size()-1; // index - genome will be the last added element to the genome vector
            seg.position = i;       // Position of where the segment started
            m_trie.insert(fragment, seg);
            i++;
        }
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return m_minimum;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    // Check for bad inputs
    if(fragment.size() < minimumLength || minimumLength < m_minimum)
        return false;
    
    matches.clear();    // clear matches
    
    // Find all possible matches starting with the minimum search size matching elements
    vector<GenomeSegment> possibleMatches = m_trie.find(fragment.substr(0, m_minimum), exactMatchOnly);
    
    // For each instance of a possible match
    for(int i=0; i< possibleMatches.size(); i++) {
        bool allowSnip = !exactMatchOnly;
        string fragmentFromGenome;
        int position = possibleMatches[i].position;
        int lengthToCheck = min( (int) fragment.size(), m_genomes[possibleMatches[i].index].length() - position);
        m_genomes[possibleMatches[i].index].extract(position, lengthToCheck, fragmentFromGenome);
        int j=0;
        for(; j < lengthToCheck; j++){
            if(fragment[j] != fragmentFromGenome[j]){
                // Break once it is no longer matching
                if(allowSnip){
                    allowSnip = false;
                }
                else{
                    break;
                }
            }
        }
        
        // If it is a match (j will represent the length of the matching portion)
        if(j >= minimumLength){
            bool pushMatch = true;
            for(int k=0; k<matches.size(); k++){
                if(matches[k].genomeName == m_genomes[possibleMatches[i].index].name()){
                    if(j <= matches[k].length){
                        pushMatch = false; // only keep the longest match from each genome
                    }
                    else{
                        // If this is a match of the same genome but larger length, update the length and position
                        matches[k].length = j;
                        matches[k].position = position;
                        pushMatch = false;
                    }
                }
            }
            
            if(pushMatch){
                DNAMatch newMatch;
                newMatch.genomeName = m_genomes[possibleMatches[i].index].name();
                newMatch.length = j;
                newMatch.position = possibleMatches[i].position;
                matches.push_back(newMatch);
            }
            
        }
    }
    
    return !matches.empty();
}

// Custom compare function to sort GenomeMatch's
bool customCompare(const GenomeMatch& a, const GenomeMatch& b) {
    if(a.percentMatch > b.percentMatch)
        return true;
    else if(b.percentMatch > a.percentMatch)
        return false;
    else{
        if(a.genomeName < b.genomeName)
            return true;
        else
            return false;
    }
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    // Handle bad cases
    if(fragmentMatchLength < minimumSearchLength())
        return false;
    
    results.clear();
    
    int S = query.length() / fragmentMatchLength;
    
    unordered_map<string, int> genomeCounts;  // Map to count how many times a genome had a match
    
    // For each sequence
    for(int i=0; i < S; i++){
        string sequence;
        query.extract(i*fragmentMatchLength, fragmentMatchLength, sequence);    // extract sequence
        vector<DNAMatch> matches;
        findGenomesWithThisDNA(sequence, fragmentMatchLength, exactMatchOnly, matches); // search for extracted sequence
        // Increase count for all found genomes
        for(int m=0; m< matches.size(); m++){
            unordered_map<string, int>::iterator it = genomeCounts.find(matches[m].genomeName);
            if(it == genomeCounts.end()){
                genomeCounts[matches[m].genomeName] = 1;
            }
            else{
                it->second++;
            }
        }
    }
    
    // for every genome g with a match
    unordered_map<string, int>::iterator it = genomeCounts.begin();
    while(it != genomeCounts.end()){
        double p = (static_cast<double>(it->second) / S ) * 100;    // compute the percentage
        if(p >= matchPercentThreshold){     // if it above the threshold, push it to the results vector
            GenomeMatch newMatch;
            newMatch.genomeName = it->first;
            newMatch.percentMatch = p;
            results.push_back(newMatch);
        }
        it++;
    }
    
    // organize the results vector by percentage with alphabetical breaking ties
    sort(results.begin(), results.end(), &customCompare);
    
    return !results.empty();
    
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
