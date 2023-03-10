#pragma once

#ifdef GAMESDB_APIKEY

#include "scrapers/Scraper.h"

namespace pugi
{
class xml_document;
}

class TheGamesDBScraper : public Scraper
{
public:
	void generateRequests(const ScraperSearchParams& params,
		std::queue<std::unique_ptr<ScraperRequest>>& requests,
		std::vector<ScraperSearchResult>& results) override;

	bool isSupportedPlatform(SystemData* system) override;
	const std::set<ScraperMediaSource>& getSupportedMedias() override;
};

class TheGamesDBJSONRequest : public ScraperHttpRequest
{
  public:
	// ctor for a GetGameList request
	TheGamesDBJSONRequest(std::queue<std::unique_ptr<ScraperRequest>>& requestsWrite,
		std::vector<ScraperSearchResult>& resultsWrite, const std::string& url)
		: ScraperHttpRequest(resultsWrite, url), mRequestQueue(&requestsWrite)
	{
	}
	// ctor for a GetGame request
	TheGamesDBJSONRequest(std::vector<ScraperSearchResult>& resultsWrite, const std::string& url)
		: ScraperHttpRequest(resultsWrite, url), mRequestQueue(nullptr)
	{
	}

  protected:
	bool process(HttpReq* request, std::vector<ScraperSearchResult>& results) override;
	bool isGameRequest() { return !mRequestQueue; }

	std::queue<std::unique_ptr<ScraperRequest>>* mRequestQueue;
};

#endif