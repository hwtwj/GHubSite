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


module.exports = router;
