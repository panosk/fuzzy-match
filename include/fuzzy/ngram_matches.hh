#pragma once

#include <fuzzy/suffix_array.hh>
#include <fuzzy/agenda_item.hh>

#include <fuzzy/tsl/hopscotch_map.h>

#include <vector>

namespace fuzzy
{
  class Range {
  public:
    Range(size_t p_start,
          size_t suffix_first, size_t suffix_last,
          size_t match_length):_p_start(p_start), _suffix_first(suffix_first),
                               _suffix_last(suffix_last), _match_length(match_length)
    {}
    size_t _p_start;
    size_t _suffix_first;
    size_t _suffix_last;
    size_t _match_length;
  };

  class NGramMatches
  {
  public:
    NGramMatches(size_t size_tm,
                 float fuzzy, unsigned p_length,
                 const SuffixArray&);

    void register_ranges(bool create, Range);
    /* same function with lazy injection feature - if match_length
       smaller than min_seq_len, we will not process the entries for the moment */
    void register_ranges(Range, unsigned min_seq_len);
    void process_backlogs();
    int get_sentence_count() const;
    std::vector<unsigned> sentence(int s_id) const;
    tsl::hopscotch_map<unsigned, AgendaItem>& get_psentences();

    unsigned max_differences_with_pattern;
    unsigned min_exact_match; // Any suffix without an subsequence of at least this with the pattern won't be accepted later

  private:
    AgendaItem* get_agendaitem(unsigned);
    AgendaItem* new_agendaitem(unsigned, unsigned);
    unsigned _p_length;
    const SuffixArray& _suffixArray;
    tsl::hopscotch_map<unsigned, AgendaItem> _psentences; // association of sentence id => AgendaItem, owns the AgendaItem
    std::vector<Range> _ranges_toprocess;
  };
}
