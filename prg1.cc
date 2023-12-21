// This is a template of the project in the course Data Structures and Algorithms. I have implemented the following functions
// excluding the random_in_range.

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here

}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

// Returns the number of affiliations.
unsigned int Datastructures::get_affiliation_count()
{

    unsigned int count = affiliations_.size();
    return count;
}

// Empties the datastructures.
void Datastructures::clear_all()
{
    affiliations_.clear();
    publications_.clear();
}

// Returns all affiliations in arbitrary order (the main program sorts them by ID).
std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    std::vector<AffiliationID> affiliation_id_v;
    affiliation_id_v.reserve(affiliations_.size());

    for (const auto& pair : affiliations_) {
        affiliation_id_v.push_back(pair.first);
    }

    return affiliation_id_v;
}

// Adds an affiliate with the given ID, name, and coordinates and returns true. 
// If the ID already has an affiliation, nothing is done and false is returned.
bool Datastructures::add_affiliation(AffiliationID id, const Name& name, Coord xy)
{
    if (affiliations_.find(id) != affiliations_.end()) {
        return false;
    }

    int distance = sqrt(xy.x * xy.x + xy.y * xy.y);

    Affiliation newAffiliation;
    newAffiliation.id_A = id;
    newAffiliation.name_A = name;
    newAffiliation.coordinates = xy;
    newAffiliation.distance = distance;

    affiliations_.insert({id, newAffiliation});
    return true;

}

// Returns the name of the affiliate with the given ID. 
// If no affiliation exists for the ID, NO_NAME is returned. 
// (The main program calls this with many IDs.)
Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto find_id = affiliations_.find(id);

    if (find_id != affiliations_.end()) {
        return find_id->second.name_A;
    }

    return NO_NAME;
}

// Returns the coordinates of the affiliate with the given ID. 
// If no affiliation exists for the ID, NO_COORD is returned. 
Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto find_coord = affiliations_.find(id);

    if (find_coord != affiliations_.end()) {
        return find_coord->second.coordinates;
    }

    return NO_COORD;
}

// Returns the IDs of the affiliations in alphabetical order according to their name. 
// Affiliations with the same name can be in an arbitrary order.
std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    std::vector<std::pair<AffiliationID, Affiliation*>> affiliations_v;
    affiliations_v.reserve(affiliations_.size());

    for (auto& pair : affiliations_) {
        affiliations_v.emplace_back(pair.first, &pair.second);
    }

    auto compareName = [](const std::pair<AffiliationID, Affiliation*>& a, const std::pair<AffiliationID, Affiliation*>& b) {
        return a.second->name_A < b.second->name_A;
    };
    std::sort(affiliations_v.begin(), affiliations_v.end(), compareName);

    std::vector<AffiliationID> sortedID_v;
    sortedID_v.reserve(affiliations_v.size());

    for (const auto& pair : affiliations_v) {
        sortedID_v.push_back(pair.first);
    }

    return sortedID_v;
}

// Returns the IDs of the affiliations in the order according to their coordinates.
// The distance is Eucledian distance. 
std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    std::vector<std::pair<AffiliationID, Affiliation*>> affiliations_v;
    affiliations_v.reserve(affiliations_.size());

    for (auto& pair : affiliations_) {
        affiliations_v.emplace_back(pair.first, &pair.second);
    }

    auto compareDistance = [](const std::pair<AffiliationID, Affiliation*>& a, const std::pair<AffiliationID, Affiliation*>& b) {
        if (a.second->distance == b.second->distance) {
            return a.second->coordinates.y < b.second->coordinates.y;
        } else {
            return a.second->distance < b.second->distance;
        }
    };

    std::sort(affiliations_v.begin(), affiliations_v.end(), compareDistance);

    std::vector<AffiliationID> sorted_ID_v;
    sorted_ID_v.reserve(affiliations_v.size());

    for (const auto& pair : affiliations_v) {
        sorted_ID_v.push_back(pair.first);
    }

    return sorted_ID_v;
}

// Return the ID of the affiliation whose coordinates match the search, if no affiliation matching the coordinates is found, 
// NO_AFFILIATION is returned.
AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    for (const auto& pair : affiliations_) {
        if (pair.second.coordinates == xy) {
            return pair.first;
        }
    }
    return NO_AFFILIATION;
}

// Changes the coordinates of the affiliation with the given ID and returns true. 
// If no affiliation is found with the ID, false is returned.
bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    auto find_affiliation = affiliations_.find(id);
    if (find_affiliation != affiliations_.end()) {
        find_affiliation->second.coordinates = newcoord;
        find_affiliation->second.distance = sqrt(newcoord.x*newcoord.x + newcoord.y*newcoord.y);
        return true;
    }

    return false;
}

// Adds the publication to the data structure with the given ID, title, Publication year 
// and associates the publication with the listed affiliations, then returns true. 
// The added publication initially has no references to other publications and no publication refers to it. 
// If the ID already has a publication, nothing is done and false is returned.
bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
    auto find_publication = publications_.find(id);
    if (find_publication != publications_.end()) {
        return false;
    }

    Publication newPublication;
    newPublication.id_P = id;
    newPublication.name_P = name;
    newPublication.year = year;

    // linking affiliationID with publication
    for ( const auto& affiliationID : affiliations) {
        auto it = affiliations_.find(affiliationID);
        if ( it != affiliations_.end()) {
            newPublication.affiliations.push_back(affiliationID);
            it->second.publications.push_back(id);
        }
    }

    publications_.insert({id, newPublication});

    return true;
}

// Returns all publication IDs in arbitrary order (the main program sorts them by ID).
std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> publications;
    publications.reserve(publications_.size());

    for (const auto& pair : publications_) {
        publications.push_back(pair.first);
    }

    return publications;
}

// Returns the title of the publication with the given ID.
// If no publication is found with the ID, NO_NAME is returned.
Name Datastructures::get_publication_name(PublicationID id)
{
    for ( const auto& pair : publications_) {
        if (pair.second.id_P == id) {
            return pair.second.name_P;
        }
    }

    return NO_NAME;
}

// Returns the publication year of the publication with the given ID. 
// If no publication is found with the ID, NO_YEAR is returned.
Year Datastructures::get_publication_year(PublicationID id)
{
    for (const auto& pair : publications_) {
        if (pair.second.id_P == id) {
            return pair.second.year;
        }
    }

    return NO_YEAR;
}

// Returns a vector containing, in arbitrary order, all affiliations related to the publication with the given ID. 
// If no publication is found with the ID, a vector whose only element is NO_AFFILATION is returned.
std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    auto find_publication = publications_.find(id);
    std::vector<AffiliationID> no_affs = {NO_AFFILIATION};
    if (find_publication != publications_.end()) {
        return find_publication->second.affiliations;

    } else {
        return no_affs;
    }
}

// The publication given as the second parameter takes the first publication as its reference and returns true. 
// If no publication exists for both IDs, nothing is done and false is returned.
bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{  
    auto find_id = publications_.find(id);
    auto find_parent = publications_.find(parentid);
    if (find_id != publications_.end() && find_parent != publications_.end()) {
        find_parent->second.references.push_back(find_id->second.id_P);
        find_id->second.parent = &(find_parent->second); // pointteri lapseen
        return true;

    } else {
        return false;
    }
}

// Returns the IDs of the publications in the vector directly referenced by the publication with the given ID. 
// If no publication is found with the given ID, a vector with NO_PUBLICATION as its only element is returned. 
// Otherwise, a vector with IDs in arbitrary order is returned.
std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    auto find_publications = publications_.find(id);
    std::vector<PublicationID> no_pubs = {NO_PUBLICATION};
    if (find_publications != publications_.end()) {
        return find_publications->second.references;
    } else {
        return no_pubs;
    }
}

// Adds the affiliate with the given ID to the publication (and of course also the other way around) and returns true. 
// If no affiliation or publication is found with the ID, nothing is done and false is returned.
bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    auto find_publication = publications_.find(publicationid);
    auto find_affiliation = affiliations_.find(affiliationid);

    if (find_publication != publications_.end() && find_affiliation != affiliations_.end()) {
        find_publication->second.affiliations.push_back(affiliationid);
        find_affiliation->second.publications.push_back(publicationid);
        return true;

    } else {
        return false;
    }
}

// Returns an arbitrarily ordered vector of post IDs to which the Affiliation with the given ID is associated. 
// (The main program arranges based on the returned ID) If no affiliation is found with the given ID, 
// a vector is returned with NO_PUBLICATION as the only element.
std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    auto find_affiliation = affiliations_.find(id);
    std::vector<PublicationID> no_pubs = {NO_PUBLICATION};
    std::vector<PublicationID> publications_v;

    if (find_affiliation != affiliations_.end()) {
        publications_v.reserve(find_affiliation->second.publications.size());

        for (const auto &pubs : find_affiliation->second.publications) {
            publications_v.push_back(pubs);
        }
        return publications_v;
    }
    return no_pubs;
}

// Returns the ID of the direct referrer (parent) of the publication with the given ID. 
// If no publication is found with the given ID, NO_PUBLICATION is returned.
PublicationID Datastructures::get_parent(PublicationID id)
{
    auto find_publication = publications_.find(id);
    if (find_publication != publications_.end() && find_publication->second.parent) {
        return find_publication->second.parent->id_P;
    }

    return NO_PUBLICATION;
}

// Returns publications related to the affiliation published in a given year or after. 
// The return vector should be arranged in ascending order according to the year of publication. 
// If several publications have been published in the same year, these should be arranged based on the ID. 
// If no affiliation is found with the given ID, a vector is returned with the pair {NO_YEAR, NO_PUBLICATION} as the only element.
std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    std::vector<std::pair<Year, PublicationID>> after_year;
    auto find_affiliation = affiliations_.find(affiliationid);

    for (const auto& pubID : find_affiliation->second.publications) {
        auto find_publication = publications_.find(pubID);
        if (find_publication != publications_.end() && find_publication->second.year >= year) {
            after_year.emplace_back(find_publication->second.year, find_publication->second.id_P);
        }
    }

    std::sort(after_year.begin(), after_year.end(), [] (const auto& a, const auto& b) {
        return a.first < b.first;
    });

    return after_year;
}

// Returns the direct and indirect Referrers (parents) of the publication with the given ID.
// That is, a direct referrer, an import referrer, etc. 
// If no publication is found with the given ID, a vector is returned with NO_PUBLICATION as its only element.
std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> chain;
    auto find_publication = publications_.find(id);

    if (find_publication != publications_.end()) {
        Publication* parent = find_publication->second.parent;

        while (parent != nullptr) {
            chain.push_back(parent->id_P);
            parent = parent->parent;
        }

    } else {
        chain.push_back(NO_PUBLICATION);
    }

    return chain;
}

// Returns, in arbitrary order, the IDs of the vector directly or indirectly referenced by the publication with the given ID. 
// (The main program sorts based on the returned ID) 
// If no publication is found with the given ID, a vector is returned, the only element of which is NO_PUBLICATION.
std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    auto find_publication = publications_.find(id);
    std::vector<PublicationID> refs_v;

    if (find_publication != publications_.end()) {
        for (const auto reference : find_publication->second.references) {
            refs_v.emplace_back(reference);
        }

        for (const auto reference : find_publication->second.references) {
            auto indirect = get_all_references(reference);
            refs_v.insert(refs_v.end(), indirect.begin(), indirect.end());
        }

    } else {
        refs_v.emplace_back(NO_PUBLICATION);
    }

    return refs_v;
}

// Removes the affiliation with the given ID and returns true. 
// Any publication produced by the affiliate may no longer list this removed affiliate in its listing. 
// If no affiliation is found with the given ID, nothing is done and If false is returned.
bool Datastructures::remove_affiliation(AffiliationID id)
{
    auto find_affiliation = affiliations_.find(id);

    if (find_affiliation != affiliations_.end()) {
        affiliations_.erase(find_affiliation);

        for (auto& publication : publications_) {
            auto& affiliations = publication.second.affiliations;
            affiliations.erase(std::remove(affiliations.begin(), affiliations.end(), id), affiliations.end());
        }

        return true;

    } else{
        return false;
    }
}
