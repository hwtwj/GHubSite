var express = require('express');
var router = express.Router();

/* GET home page. */
router.get("/", function (req, res) {
    "use strict";
    res.render('index.ejs');
});

router.get("/slam", function (req, res) {
    "use strict";
    res.render('slam.ejs');
});

router.get("/magic3d", function (req, res) {
    "use strict";
    res.render('magic3d.ejs');
});

router.get("/bgl/document", function (req, res) {
    "use strict";
    res.render('bgldocument.ejs');
});

router.get("/geometryplusplus", function (req, res) {
    "use strict";
    res.render('gppabout.ejs');
});

router.get("/geometryplusplus/feature", function (req, res) {
    "use strict";
    res.render('gppfeature.ejs');
});

router.get("/geometryplusplus/document", function (req, res) {
    "use strict";
    res.render('gppdocument.ejs');
});

router.get("/geometryplusplus/fileformat", function (req, res) {
    "use strict";
    res.render('gpp_fileformat.ejs');
});

router.get("/geometryplusplus/pointcloudbasic", function (req, res) {
    "use strict";
    res.render('gpp/gpp_pointcloudbasic.ejs');
});

router.get("/geometryplusplus/meshbasic", function (req, res) {
    "use strict";
    res.render('gpp/gpp_meshbasic.ejs');
});

router.get("/geometryplusplus/meshadvanced", function (req, res) {
    "use strict";
    res.render('gpp/gpp_meshadvanced.ejs');
});

router.get("/geometryplusplus/meshcurve", function (req, res) {
    "use strict";
    res.render('gpp/gpp_meshcurve.ejs');
});

router.get("/geometryplusplus/pointcloudregistration", function (req, res) {
    "use strict";
    res.render('gpp/gpp_pointcloudregistration.ejs');
});

router.get("/geometryplusplus/measuremodule", function (req, res) {
    "use strict";
    res.render('gpp/gpp_measuremodule.ejs');
});

router.get("/geometryplusplus/texturemodule", function (req, res) {
    "use strict";
    res.render('gpp/gpp_texturemodule.ejs');
});

router.get("/geometryplusplus/uvunfold", function (req, res) {
    "use strict";
    res.render('gpp/gpp_uvunfold.ejs');
});

router.get("/geometryplusplus/nonrigid", function (req, res) {
    "use strict";
    res.render('gpp/gpp_nonrigid.ejs');
});

router.get("/demo/overview", function (req, res) {
    "use strict";
    res.render('demo_overview.ejs');
});

router.get("/demo/workflow", function (req, res) {
    "use strict";
    res.render('demo_workflow.ejs');
});

router.get("/demo/deformation", function (req, res) {
    "use strict";
    res.render('demo_deformation.ejs');
});

router.get("/demo/registration", function (req, res) {
    "use strict";
    res.render('demo_registration.ejs');
});

router.get("/demo/pointshop", function (req, res) {
    "use strict";
    res.render('demo_pointshop.ejs');
});

router.get("/demo/meshshop", function (req, res) {
    "use strict";
    res.render('demo_meshshop.ejs');
});

router.get("/demo/measure", function (req, res) {
    "use strict";
    res.render('demo_measure.ejs');
});

router.get("/demo/unfold", function (req, res) {
    "use strict";
    res.render('demo_unfold.ejs');
});

router.get("/demo/texture", function (req, res) {
    "use strict";
    res.render('demo_texture.ejs');
});

router.get("/wangqifeng", function (req, res) {
    "use strict";
    res.render('wangqifeng.ejs');
});

module.exports = router;
