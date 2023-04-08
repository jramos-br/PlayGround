using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Threading.Tasks;
using System.Web.Http;
using System.Web.Http.Cors;

namespace BNYConstituicaoFundos.Controllers
{
   
    [EnableCors(origins: "http://177.44.44.121:8080", headers: "*", methods: "*")]
    public class TokenController : ApiController
    {
        HttpClient client = new HttpClient();
        public TokenController()
        {
            HttpClient client = new HttpClient();
            client.DefaultRequestHeaders.Accept.Clear();
            client.DefaultRequestHeaders.Add("Accept", "application/json");
        }

        public async Task<Object> Get()
        {
            //Credenciais em formato string e modelo json
            string credentials = "{'client_id':'[key client ID', 'client_secret':'[client secret key]'}";
            var contentHTTP = new StringContent(credentials, Encoding.UTF8, "application/json"); // conversão da string para json
            contentHTTP.Headers.ContentType = new MediaTypeHeaderValue("application/json");
            //Request Token (requisição assincrona do token)
            HttpResponseMessage response = await client.PostAsync("https://authuat.springcm.com/api/v201606/apiuser", contentHTTP);
            return response;
        }
    }
}
