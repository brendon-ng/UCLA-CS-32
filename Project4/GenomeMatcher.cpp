#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
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
        while(i + m_minimum <= genome.length()){
            string fragment;
            genome.extract(i, m_minimum, fragment);
            GenomeSegment seg;
            seg.index = (int) m_genomes.size()-1;
            seg.position = i;
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
        m_genomes[possibleMatches[i].index].extract(possibleMatches[i].position, (int) fragment.size(), fragmentFromGenome);
        int j=0;
        for(; j<fragment.length(); j++){
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
                        matches[k].position = possibleMatches[i].position;
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

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    if(fragmentMatchLength < minimumSearchLength())
        return false;
    
    int S = query.length() % fragmentMatchLength;
    int pos = 0;
    
    int matchesFound = 0;
    for(int i=0; i < S; i++){
        string sequence;
        query.extract(i, fragmentMatchLength, sequence);
        vector<DNAMatch> matches;
        findGenomesWithThisDNA(sequence, fragmentMatchLength, exactMatchOnly, matches);
        double p = (matches.size() / S) * 100;
        if (p >= matchPercentThreshold){
            GenomeMatch gMatch;
            gMatch.genomeName = "wfwef";
            results.push_back(GenomeMatch());
        }
        
        
        pos+=fragmentMatchLength;
    }
    return true;
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
