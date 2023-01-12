import { Helmet } from "react-helmet-async";
// @mui
import { useTheme, createTheme } from "@mui/material/styles";
import { Grid, Container } from "@mui/material";
// components

import React, { useState, useEffect } from "react";
import PropTypes from "prop-types";
import { makeStyles } from "@material-ui/core/styles";
import LinearProgress from "@material-ui/core/LinearProgress";
import Box from "@material-ui/core/Box";
import { Card, CardMedia, CardContent } from "@mui/material";
// mock
// ----------------------------------------------------------------------
import $ from 'jquery';

import QuestionCard from '../../components/QuestionCard';

import { theme } from '../../data/AppTheme'
import { createGlobalStyle, ThemeProvider } from 'styled-components';
import { useLocation, Link } from "react-router-dom";

const staticURL = 'http://localhost:8000/static/';


function LinearProgressWithLabel(props) {
    return (
        <Box display="flex" alignItems="center" p={3}>
            <Box width="100%" mr={3} >
                <LinearProgress variant="determinate" style={{ height: '40px', borderRadius: '25px', backgroundColor: `#eeeeee` }} {...props} />
            </Box>
            <Box minWidth={35}>
                {`${Math.round(
                    props.value
                )}%`}
            </Box>
        </Box>
    );
}


const styles = props => ({
    colorPrimary: {
        backgroundColor: '#00695C',
    },
    barColorPrimary: {
        backgroundColor: '#B2DFDB',
    }
});

LinearProgressWithLabel.propTypes = {
    /**
     * The value of the progress indicator for the determinate and buffer variants.
     * Value between 0 and 100.
     */
    value: PropTypes.number.isRequired
};

const useStyles = makeStyles({
    root: {
        width: "100%"
    }
});


function Recognition(score, scoreChanger, indexChanger) {
    return (
        <>
            <ThemeProvider theme={theme}>
                <QuestionCard gamemode='recognition' scoreChanger={scoreChanger} score={score} indexChanger={indexChanger} />
            </ThemeProvider>
        </>
    );
}
function Translation(score, scoreChanger, indexChanger) {
    return (
        <>
            <ThemeProvider theme={theme}>
                <QuestionCard gamemode='translation' scoreChanger={scoreChanger} score={score} indexChanger={indexChanger} />
            </ThemeProvider>
        </>
    );
}

var timeouts = [];



function Matching(setScore, setIndex) {

    let total_time = 1000;
    let symbols = ['a', 'b', 'c', 'd', 'e', 'f', 'i', 'j', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 't', 'v', 'w', 'x', 'y', 'z'];
    //let symbols = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'];
    //some symbols are hard to get right
    let user_score = 0;

    let start_time = new Date().getTime() / 1000;
    let asl_interval = setInterval(function () {
        $.get('http://localhost:8000/current', '', function (data, textStatus, jqXHR) {
            if (data.new) {
                let target = $('#prompt3').attr('class');
                console.log(data['symbol'], target)
                if (data['symbol'] == target) {
                    console.log(2)
                    let symbolIndex = Math.floor(Math.random() * symbols.length);

                    // Same character, try the next one
                    if (symbols[symbolIndex] == target) {
                        symbolIndex = (symbolIndex + 1)
                    }
                    symbolIndex = symbolIndex % symbols.length;
                    let nextSymbol = symbols[symbolIndex];
                    var newText2 = 'Great job!'
                    var newText = 'Now try';
                    var newText3 = nextSymbol;
                    setIndex((prev) => prev + 1);
                    $('#prompt3').attr('class', nextSymbol);
                    $('#realimage').attr('src', staticURL + 'topic/alphabet/' + nextSymbol.toUpperCase() + '.png');

                    setScore((prev) => prev + 1);

                } else {
                    var newText1 = data.symbol;
                }
                $('#prompt').text(newText);
                $('#prompt1').text(newText1);
                $('#prompt2').text(newText2);
                $('#prompt3').text(newText3.toUpperCase());


            }
        });
    }, 200);

    timeouts.push(asl_interval);

    setInterval(function () {
        var current_time = new Date().getTime() / 1000;
        var timeleft = Math.floor(total_time + (start_time - current_time));
        if (timeleft < 0) {
            timeleft = 0;
        }
        $('#time-left').text(timeleft);
    }, 1000);

    setTimeout(function () {
        clearInterval(asl_interval);
        game_over_screen();
    }, total_time * 1000);

    function game_over_screen() {
        $("#container").fadeOut();
        $("#topbar").fadeOut();
        $('#point_amount').text(user_score.toString());
        $("#myModal").modal('show');
    }

    window.submit_score = function submit_score() {
        var name = $("#username").val();
        $.post('/score/', JSON.stringify({ 'score': user_score, 'user': name }));
        $.get('/scores/', '', function (scores) {
            console.log(scores['scores'].length);
            for (var i = 0; i < scores['scores'].length; i++) {
                console.log(scores['scores'][i]);
                $("#scoreboard").append("<tr><td>" + scores['scores'][i].user + "</td><td>" + scores['scores'][i].score + "</td></tr>");
            }
        });

        $("#scoreboard").removeClass('hidden');
        $("#scoreboard_title").removeClass('hidden');
        $("#back_to_splash_link").removeClass('hidden');
    }

    return (

        <Container maxWidth="xl" sx={{ marginTop: '20px' }}>
            <Grid container>
                <Grid item xs={6}>
                    <Card style={{ backgroundColor: '#b4efed' }}>
                        <Grid
                            container
                            sx={{
                                textAlign: 'center', // <-- the magic
                                fontWeight: 'bold',
                                fontSize: 100,
                                height: '85vh',
                                fontFamily: 'righteous',
                                color: '#0db2ad',
                            }}
                        >
                            <Grid item xs={12}>
                                <span id="prompt2" > &nbsp;</span>

                            </Grid>
                            <Grid item xs={12}>
                                <span id="prompt">Make a </span>

                            </Grid>
                            <Grid item xs={12}>
                                <span id="prompt3" style={{color:"#5f27cd", fontSize:200}} className='b'>B</span>

                            </Grid>
                            <Grid item xs={12}>
                                <span id="prompt1"  >&nbsp;</span>

                            </Grid>

                        </Grid>
                    </Card>
                </Grid>
                <Grid item xs={6}>
                    <Card sx={{ display: 'flex', flexDirection: 'column', height: '100%', backgroundColor: '#bbaae5' }}>
                        <CardMedia
                            id="realimage"
                            component="img"
                            image={staticURL + 'topic/alphabet/B.png'}
                            alt="placeholder"
                            sx={{
                                padding: "1em 1em 0 1em", objectFit: "contain", width: '100%', height: '75vh',
                                margin: "0 auto"
                            }}
                        />


                    </Card>
                </Grid>
            </Grid>

        </Container>
    );
}


export default function TopicPage() {
    const [score, setScore] = useState(0);
    const [index, setIndex] = useState(0);

    const classes = useStyles();


    const location = useLocation();
    const state = location.state;

    var game;
    for (var i = 0; i < timeouts.length; i++) {
        clearTimeout(timeouts[i]);
    }
    timeouts = [];
    if (state[1] == 'Matching') {
        game = Matching(setScore, setIndex);
    } else if (state[1] == 'Recognition') {
        game = Recognition(score, setScore, setIndex);
    } else if (state[1] == 'Translation') {
        game = Translation(score, setScore, setIndex);
    }

    useEffect(() => {
        $('.MuiLinearProgress-barColorPrimary').css('background-color', "#00d2d3")
    }, [])
    return (
        <>
            <Container maxWidth="xl" >

                <Helmet>
                    <title> SignUp! </title>
                </Helmet>
                <Container maxWidth="xl" sx={{ marginTop: '20px' }}>
                    <Grid container spacing={2}>
                        <Grid item xs={10}>
                            <Card>
                                <div className={classes.root}>
                                    <LinearProgressWithLabel value={(index / 50) * 100} />
                                </div>
                            </Card>
                        </Grid>
                        <Grid item xs={2}>
                            <Card sx={{ height: '100%' }}>
                                <Box
                                    display="flex"
                                    justifyContent="center"
                                    alignItems="center"
                                    height="100%"
                                >
                                    <span id='user_score_string'>
                                        Score: &nbsp;
                                    </span>
                                    <span id='user_score'>
                                        {score}
                                    </span>
                                </Box>
                            </Card>
                        </Grid>
                    </Grid>
                </Container>
                {game}


            </Container>
        </>
    );
}
