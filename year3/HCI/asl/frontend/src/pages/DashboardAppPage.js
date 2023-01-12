import { Helmet } from "react-helmet-async";
import { faker } from "@faker-js/faker";
// @mui
import { useTheme } from "@mui/material/styles";
import { Grid, Container, Typography } from "@mui/material";
// components
import Iconify from "../components/iconify";

// sections
import {
    AppNewsUpdate,
    AppWebsiteVisits,
    AppWidgetSummary,
    AppCurrentSubject,
} from "../sections/@dashboard/app";

import { Card, CardHeader, Box, Button } from "@mui/material";
import { Button as Button2 } from '../components/PageElements/UIElements';

// ----------------------------------------------------------------------

export default function DashboardAppPage() {

    const theme = useTheme();
    return (
        <>
            <Helmet>
                <title> SignUp! </title>
            </Helmet>
            <Container maxWidth="xl">
                <Grid container spacing={3} style={{ height: "100%" }}>


                    <Grid item xs={12} md={6} lg={8}>
                        <AppWebsiteVisits
                            title="Performance"
                            chartLabels={[
                                "1",
                                "2",
                                "3",
                                "4",
                                "5",
                                "6",
                                "7",
                                "8",
                                "9",
                                "10",
                            ]}
                            chartData={[
                                {
                                    name: "Average time per question (seconds)",
                                    type: "column",
                                    fill: "solid",
                                    data: [23, 11, 22, 27, 13, 22, 37, 21, 44, 22],
                                },
                                {
                                    name: "Average correct answers (%)",
                                    type: "area",
                                    fill: "gradient",
                                    data: [44, 55, 41, 67, 22, 43, 21, 41, 56, 27],
                                },
                                {
                                    name: "Number of questions completed",
                                    type: "line",
                                    fill: "solid",
                                    data: [30, 25, 36, 30, 45, 35, 64, 52, 59, 36],
                                },
                            ]}
                        />
                    </Grid>


                    <Grid item xs={12} md={6} lg={4} >
                        <a href="../application" >
                            <Button2 style={{
                                height: "100%", border: 'solid 7px #5f27cd',
                                borderRadius: '20px',
                                background: '#5f27cd00',
                            }} >
                                <Typography variant="h1" sx={{ fontFamily: 'Righteous, cursive', color: '#5f27cd' }}>PLAY</Typography>
                            </Button2>
                        </a>
                    </Grid>
                    <Grid item xs={12}>
                        <Card>
                            <CardHeader title={"Topics"} />
                            <Box sx={{ p: 3, pb: 3 }} dir="ltr">
                                <Grid container spacing={1}>
                                    <Grid item xs={3}>
                                        <AppWidgetSummary
                                            title="Alphabet"
                                            color="teal"
                                            icon={"mdi:alphabetical-variant"}
                                        />
                                    </Grid>
                                    <Grid item xs={3}>
                                        <AppWidgetSummary
                                            title="Animals"
                                            color="teal"
                                            icon={"cil:animal"}
                                        />
                                    </Grid>
                                    <Grid item xs={3}>
                                        <AppWidgetSummary
                                            title="Food"
                                            color="teal"
                                            icon={"dashicons:food"}
                                        />
                                    </Grid>
                                    <Grid item xs={3}>
                                        <AppWidgetSummary
                                            title="Home"
                                            color="teal"
                                            icon={"bxs:home"}
                                        />
                                    </Grid>
                                </Grid>
                            </Box>
                        </Card>
                    </Grid>
                    {/*
                    <Grid item xs={12} md={6} lg={4}>
                        <AppCurrentSubject
                            title="Current Subject"
                            chartLabels={[
                                "English",
                                "History",
                                "Physics",
                                "Geography",
                                "Chinese",
                                "Math",
                            ]}
                            chartData={[
                                { name: "Series 1", data: [80, 50, 30, 40, 100, 20] },
                                { name: "Series 2", data: [20, 30, 40, 80, 20, 80] },
                                { name: "Series 3", data: [44, 76, 78, 13, 43, 10] },
                            ]}
                            chartColors={[...Array(6)].map(
                                () => theme.palette.text.secondary
                            )}
                        />
                    </Grid>

                    <Grid item xs={12} md={6} lg={8}>
                        <AppNewsUpdate
                            title="Activity"
                            list={[...Array(5)].map((_, index) => ({
                                id: faker.datatype.uuid(),
                                title: faker.name.jobTitle(),
                                description: faker.name.jobTitle(),
                                image: `/assets/images/covers/cover_${index + 1}.jpg`,
                                postedAt: faker.date.recent(),
                            }))}
                        />
                    </Grid>
                        */}
                </Grid>
            </Container>
        </>
    );
}
