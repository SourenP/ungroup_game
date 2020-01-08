/**
 * Keeps track of a metric temporally in order to produce it's rate over an interval of time.
 *
 * Counts are stored in a specified number of slots each representing an equal amount of time.
 * Counts are accumilated in a slot until the interval of time has passed, at which point counts
 * start accumilating in a new slot. There are a limited number of slots and each time a new slot is
 * used, the oldest is discarded.
 *
 * This class currenly only supports uint counts.
 */

#include <SFML/System.hpp>
#include <boost/circular_buffer.hpp>

#ifndef TemporalMetric_hpp
#define TemporalMetric_hpp

class TemporalMetric {
  public:
    /**
     * @size: Number of slots of time to store at once.
     * @time_granularity: The amount of time each slot represents.
     */
    explicit TemporalMetric(size_t size, sf::Time time_granularity);
    ~TemporalMetric(){};
    TemporalMetric(const TemporalMetric& temp_obj) = delete;
    TemporalMetric& operator=(const TemporalMetric& temp_obj) = delete;

    /**
     * Updates the slot currently accumilating the metric if need be.
     * Should be called as often as possible.
     */
    void update();

    /**
     * Increment the count of the metric by one.
     */
    void incrementCount();

    /**
     * Increment the count of the metric by @n.
     */
    void incrementCount(uint n);

    /**
     * Get rate of metric per @interval amount of time
     */
    float getRate(sf::Time interval);

  private:
    sf::Clock m_clock;
    boost::circular_buffer<uint> m_counts;
    sf::Time m_timeGranularity;
    uint m_currentCount = 0;
};

#endif /* TemporalMetric_hpp */
