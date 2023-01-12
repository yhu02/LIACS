import { Helmet } from "react-helmet-async";
import { faker } from "@faker-js/faker";
// @mui
import { useTheme } from "@mui/material/styles";
import { Grid, Container, Typography } from "@mui/material";
// components
import { useLocation, Link } from "react-router-dom";
// sections
import {
    AppWidgetSummary,
} from "../../sections/@dashboard/app";

import { Card, CardHeader, Box } from "@mui/material";
import { Button } from '../../components/PageElements/UIElements';
import { useState, useEffect } from 'react'

let basecolor = 'teal'

const config = [
    {
        title: 'Alphabet 1',
        color: basecolor,
        icon: 'mdi:alphabetical-variant',
    },
    {
        title: 'Animals',
        color: basecolor,
        icon: 'cil:animal',
    },
    {
        title: 'Food',
        color: basecolor,
        icon: 'dashicons:food',
    },
];

// ----------------------------------------------------------------------
export default function TopicPage() {
    const theme = useTheme();

    const location = useLocation();
    const state = location.state;

    const [item_, setItem] = useState(0);
    const [last, setLast] = useState(null);
    const [previous, setPrevious] = useState(null);
    const [bg, setBg] = useState(null);

    useEffect(() => {
        if (last != null) {           
            setBg(last.style.background);
      
            last.style.background = '#5f27cd'
            if (previous != null) {
                previous.style.background = bg;
            }else{
               
            }
            setPrevious(last)

        }
    }, [last]);

    useEffect(() => {
 
    }, [previous]);

    const onclickFunction = (e, item) => {
        setItem(item);
        setLast(e.currentTarget);
    }
    return (
        <>
            <Helmet>
                <title> SignUp! </title>
            </Helmet>
            <Container maxWidth="xl">
                <Grid container spacing={3} direction='row' sx={{ height: '100vh' }} >
                    <Grid item xs={12} style={{ height: "80%" }}>
                        <Card style={{ height: "100%" }}>
                            <CardHeader title={"Select Topic"} />
                            <Box sx={{ p: 3, pb: 1 }} dir="ltr" >
                                <Grid container spacing={2}  >
                                    {
                                        config.map((item) => (
                                            <Grid item xs={4}>
                                                    <AppWidgetSummary sx={{
                                                        py: '50%',
                                                    }}
                                                        {...item}
                                                        onClick={(e) => { onclickFunction(e, item) }}

                                                    />
                                            </Grid>
                                        ))
                                    }
                                </Grid>
                            </Box>
                        </Card>
                    </Grid>
                    <Grid item xs={12} style={{ height: "20%" }}>
                        <Link to="../gamemode" state={[state, item_.title]} style={{ textDecoration: 'none' }}>

                                <Button style={{ height: "100%", fontFamily: 'Righteous, cursive', color: '#5f27cd', fontSize: '36px',border: 'solid 7px #5f27cd',
                                borderRadius: '20px',
                                background: '#5f27cd00', }}>Next</Button>
                        </Link>

                    </Grid>
                </Grid>
            </Container>
        </>
    );
}
