﻿namespace AspNetCoreBoilerplateApplication1.Services
{
    using System;
    using System.Collections.Generic;
    using System.Threading.Tasks;
    using Boilerplate.AspNetCore;
    using Boilerplate.AspNetCore.Caching;
    using Boilerplate.AspNetCore.Sitemap;
    using Microsoft.AspNetCore.Mvc;
    using Microsoft.Extensions.Caching.Distributed;
    using Microsoft.Extensions.Logging;
    using Microsoft.Extensions.Options;
    using AspNetCoreBoilerplateApplication1.Constants;
    using AspNetCoreBoilerplateApplication1.Settings;

    /// <summary>
    /// Generates sitemap XML for the current site.
    /// </summary>
    public class SitemapService : SitemapGenerator, ISitemapService
    {
        private readonly IOptionsSnapshot<CacheProfileSettings> cacheProfileSettings;
        private readonly IDistributedCache distributedCache;
        private readonly ILogger<SitemapService> logger;
        private readonly IUrlHelper urlHelper;

        /// <summary>
        /// Initializes a new instance of the <see cref="SitemapService" /> class.
        /// </summary>
        /// <param name="cacheProfileSettings">The cache profile settings.</param>
        /// <param name="distributedCache">The distributed cache for the application.</param>
        /// <param name="logger">The <see cref="SitemapService"/> logger.</param>
        /// <param name="urlHelper">The URL helper.</param>
        public SitemapService(
            IOptionsSnapshot<CacheProfileSettings> cacheProfileSettings,
            IDistributedCache distributedCache,
            ILogger<SitemapService> logger,
            IUrlHelper urlHelper)
        {
            this.cacheProfileSettings = cacheProfileSettings;
            this.distributedCache = distributedCache;
            this.logger = logger;
            this.urlHelper = urlHelper;
        }

        /// <summary>
        /// Gets the sitemap XML for the current site. If an index of null is passed and there are more than 25,000
        /// sitemap nodes, a sitemap index file is returned (A sitemap index file contains links to other sitemap files
        /// and is a way of splitting up your sitemap into separate files). If an index is specified, a standard
        /// sitemap is returned for the specified index parameter. See http://www.sitemaps.org/protocol.html
        /// </summary>
        /// <param name="index">The index of the sitemap to retrieve. <c>null</c> if you want to retrieve the root
        /// sitemap or sitemap index document, depending on the number of sitemap nodes.</param>
        /// <returns>The sitemap XML for the current site or <c>null</c> if the sitemap index is out of range.</returns>
        public async Task<string> GetSitemapXml(int? index = null)
        {
            // Here we are caching the entire set of sitemap documents. We cannot use the caching attribute because
            // cache expiry could get out of sync if the number of sitemaps changes.
            var sitemapDocuments = await this.distributedCache.GetAsJsonAsync<List<string>>(CacheProfileName.SitemapNodes);
            if (sitemapDocuments == null)
            {
                IReadOnlyCollection<SitemapNode> sitemapNodes = this.GetSitemapNodes();
                sitemapDocuments = this.GetSitemapDocuments(sitemapNodes);
                await this.distributedCache.SetAsJsonAsync(
                    CacheProfileName.SitemapNodes,
                    sitemapDocuments,
                    options: new DistributedCacheEntryOptions()
                    {
                        AbsoluteExpirationRelativeToNow = this.GetExpirationDuration()
                    });
            }

            if (index.HasValue && ((index < 1) || (index.Value >= sitemapDocuments.Count)))
            {
                return null;
            }

            return sitemapDocuments[index.HasValue ? index.Value : 0];
        }

        /// <summary>
        /// Gets a collection of sitemap nodes for the current site.
        /// TODO: Add code here to create nodes to all your important sitemap URL's.
        /// You may want to do this from a database or in code.
        /// </summary>
        /// <returns>A collection of sitemap nodes for the current site.</returns>
        protected virtual IReadOnlyCollection<SitemapNode> GetSitemapNodes()
        {
            List<SitemapNode> nodes = new List<SitemapNode>();

            nodes.Add(
                new SitemapNode(this.urlHelper.AbsoluteRouteUrl(HomeControllerRoute.GetIndex))
                {
                    Priority = 1
                });
            nodes.Add(
               new SitemapNode(this.urlHelper.AbsoluteRouteUrl(HomeControllerRoute.GetAbout))
               {
                   Priority = 0.9
               });
            nodes.Add(
               new SitemapNode(this.urlHelper.AbsoluteRouteUrl(HomeControllerRoute.GetContact))
               {
                   Priority = 0.9
               });

            // An example of how to add many pages into your sitemap.
            // foreach (int productId in myProductRepository.GetProductIds())
            // {
            //     nodes.Add(
            //        new SitemapNode(this.urlHelper.AbsoluteRouteUrl(ProductControllerRoute.GetProduct, new { id = productId }))
            //        {
            //            Frequency = SitemapFrequency.Weekly,
            //            LastModified = DateTime.Now,
            //            Priority = 0.8
            //        });
            // }

            return nodes;
        }

        protected override string GetSitemapUrl(int index)
        {
            return this.urlHelper.AbsoluteRouteUrl(HomeControllerRoute.GetSitemapXml).TrimEnd('/') + "?index=" + index;
        }

        protected override void LogWarning(Exception exception)
        {
            this.logger.LogWarning(exception.Message, exception);
        }

        private TimeSpan GetExpirationDuration()
        {
            var cacheProfile = this.cacheProfileSettings.Value.CacheProfiles[CacheProfileName.SitemapNodes];
            return TimeSpan.FromSeconds(cacheProfile.Duration.Value);
        }
    }
}