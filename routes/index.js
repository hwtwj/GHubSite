var express = require('express');
var router = express.Router();

/* GET home page. */
router.get("/", function (req, res) {
    "use strict";
    res.render('index.ejs');
});

router.get("/myspace", function (req, res) {
    "use strict";
    res.render('myspace.ejs');
});

router.get("/slam", function (req, res) {
    "use strict";
    res.render('slam.ejs');
});

router.get("/magic3d", function (req, res) {
    "use strict";
    res.render('magic3d.ejs');
});

router.get("/cagemodeling", function (req, res) {
    "use strict";
    "use strict";
    var login;
    if (req.session.user === undefined) {
        login = false;
    } else {
        login = true;
    }
    if (req.query.designId === undefined || req.query.designId === "") {
        res.render("cagemodeling.ejs", {designId: "", login: login});
    } else {
        res.render("cagemodeling.ejs", {designId: req.query.designId, login: login});
    }
});

router.get("/bgl/document", function (req, res) {
    "use strict";
    res.render('bgldocument.ejs');
});

router.get("/testformat", function (req, res) {
    "use strict";
    res.render('testformat.ejs');
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
    res.render('gpp/gpp_fileformat.ejs');
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

router.get("/notes", function (req, res) {
    "use strict";
    res.render('notes/notes.ejs');
});

router.get("/notes/3darchitecture", function (req, res) {
    "use strict";
    res.render('notes/3darchitecture.ejs');
});

router.get("/notes/debugprogram", function (req, res) {
    "use strict";
    res.render('notes/debugprogram.ejs');
});

router.get("/notes/pointcloudrepresentation", function (req, res) {
    "use strict";
    res.render('notes/pointcloudrepresentation.ejs');
});

router.get("/notes/pointcloudnormal", function (req, res) {
    "use strict";
    res.render('notes/pointcloudnormal.ejs');
});

router.get("/notes/transform", function (req, res) {
    "use strict";
    res.render('notes/transform.ejs');
});

router.get("/notes/3dfeature", function (req, res) {
    "use strict";
    res.render('notes/3dfeature.ejs');
});

router.get("/notes/registration", function (req, res) {
    "use strict";
    res.render('notes/registration.ejs');
});

router.get("/notes/kinectdata", function (req, res) {
    "use strict";
    res.render('notes/kinectdata.ejs');
});

router.get("/notes/stlformat", function (req, res) {
    "use strict";
    res.render('notes/stlformat.ejs');
});

router.get("/notes/meshdatastructure", function (req, res) {
    "use strict";
    res.render('notes/meshdatastructure.ejs');
});

router.get("/notes/measuremesh", function (req, res) {
    "use strict";
    res.render('notes/measuremesh.ejs');
});

router.get("/notes/uvunfold", function (req, res) {
    "use strict";
    res.render('notes/uvunfold.ejs');
});

router.get("/notes/colormesh", function (req, res) {
    "use strict";
    res.render('notes/colormesh.ejs');
});

router.get("/notes/3dpicking", function (req, res) {
    "use strict";
    res.render('notes/3dpicking.ejs');
});

router.get("/notes/delaunaytriangulation", function (req, res) {
    "use strict";
    res.render('notes/delaunaytriangulation.ejs');
});

router.get("/notes/subdividemodeling", function (req, res) {
    "use strict";
    res.render('notes/subdividemodeling.ejs');
});

router.get("/notes/patternrecognition", function (req, res) {
    "use strict";
    res.render('notes/patternrecognition.ejs');
});

router.get("/notes/numericaloptimization", function (req, res) {
    "use strict";
    res.render('notes/numericaloptimization.ejs');
});

router.get("/scandata", function (req, res) {
    "use strict";
    res.render('scandata.ejs');
});

module.exports = router;
