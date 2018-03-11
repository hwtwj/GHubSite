var express = require('express');
var router = express.Router();

/* GET home page. */
router.get("/", function (req, res) {
    "use strict";
    if (req.session.user === undefined) {
        res.render('index.ejs', {login: false, username: ""});
    } else {
        res.render('index.ejs', {login: true, username: req.session.user});
    }
});

module.exports = router;
