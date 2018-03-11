var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
// var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var session = require('express-session');
var routes = require('./routes/index');
var users = require('./routes/users');
var log4js = require('log4js');
log4js.configure({
    appenders: [
    { type: 'console', backup: 1024 },
    { type: 'file', filename: 'logs/general.log', maxLogSize: 10485760, backup: 1024, level: "ERROR", category: ['general', 'console'] }
    ],
    replaceConsole: true
});
var logger = log4js.getLogger("general");
logger.setLevel("INFO");

var app = express();

// view engine setup
// var engines = require('consolidate');
// app.engine('ejs', engines.ejs);
// app.engine('jade', engines.jade);
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

// uncomment after placing your favicon in /public
//app.use(favicon(__dirname + '/public/favicon.ico'));
// app.use(logger('dev'));
app.use(log4js.connectLogger(logger, { level: 'auto', format:':remote-addr :referrer :method :url :status :response-time' }));
app.use(bodyParser.urlencoded({limit: '20mb', parameterLimit: 20000000, extended: false}));
app.use(bodyParser.json({limit: '20mb'}));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(session({
    secret: 'ccc3412silence',
    resave: false,
    saveUninitialized: true
}));

app.use('/', routes);
app.use('/users', users);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
    app.use(function(err, req, res, next) {
        res.status(err.status || 500);
        res.render('error.ejs', {
            message: err.message,
            error: err
        });
    });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error.ejs', {
        message: err.message,
        error: {}
    });
});


module.exports = app;
