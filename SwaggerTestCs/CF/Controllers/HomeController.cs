using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace BNYConstituicaoFundos.Controllers
{
    public class HomeController : Controller
    {
        public ActionResult Index()
        {
            var page = new Pages();
            page.Title = "Constituição de Fundos";
            page.subtitulo = "";
            return View(page);
        }

        public ActionResult ConstFundo()
        {
            ViewBag.Title = "Constituição de Fundos - Gestor";

            return View();
        }

        public ActionResult Faixas()
        {
            return View();
        }

    }
}