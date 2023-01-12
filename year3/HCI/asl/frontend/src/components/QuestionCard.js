import React, { useEffect, useState } from 'react'
import ShowMessage from './PageElements/ShowMessage';
import { Button } from './PageElements/UIElements';
import ShowConfetti from './ShowConfetti';
import { Container, Grid, Box, CardMedia, Card } from '@mui/material';
import $ from 'jquery';
import Typography from 'src/theme/overrides/Typography';

const staticURL = 'http://localhost:8000/static/';

//<img id='question-img'className='question-text' src={staticURL + randQustions[currentIndex].question} />


var randQustions
var selected_topics = ['alphabet']
const quiz_data_call = (gamemode) => {
   let n_answers = 0;
   if (gamemode == 'translation') {
      n_answers = 4
   } else if (gamemode == 'recognition') {
      n_answers = 3
   }
   $.ajax({
      type: 'POST',
      url: 'http://localhost:8000/quiz_data/',
      data: { "topic": selected_topics, 'n_answers': n_answers },
      success: function (data) {
         randQustions = data
      },
      dataType: '',
      async: false
   });

}
var last;
var timer;

export default function QuestionCard({ gamemode, ...props }) {
   var setScore = props.scoreChanger
   var score = props.score
   var setIndex = props.indexChanger
   const [currentIndex, setCurrentIndex] = useState(0)
   const [endQuiz, setEndQuiz] = useState(false)
   const [avg, setAvg] = useState(0);
   const [showAns, setShowAns] = useState(false);
   const [block, setBlock] = useState(false); // requery data
   const [block2, setBlock2] = useState(false); // block double render

   useEffect(() => {
      if (block2 == true) {
         setBlock(false);
      }
      setBlock2(true);
   }, [gamemode]);

   if (block == false) {
      quiz_data_call(gamemode)
      setBlock(true);
   }

   useEffect(() => {

      setIndex(currentIndex)
      if (endQuiz == false) {
         timer = setTimeout(() => {
            handleAnswerClick(false)
         }, 5000);
         return () => clearTimeout(timer);
      }

   }, [currentIndex, endQuiz]);

   //function to calc score and show correct answer
   const handleAnswerClick = (isCorrect, e) => {
      setShowAns(true)
      const event = e
      let orig_background;

      if (e) {
         clearTimeout(timer);
         last = e.currentTarget
         orig_background = e.currentTarget.style.background
      }

      if (isCorrect) {
         setScore((prev) => prev + 1);

      } else if (e) {
         e.currentTarget.style.background = '#f97794'
      }

      setTimeout(() => {
         last.style.background = orig_background
         nextQuestion()
      }, 1800)

   }

   //function to go to next question if within bounds of quiz length and hide correct answer 
   // if ouside the bounds of the quiz array, the quiz will end
   const nextQuestion = () => {
      setShowAns(false)
      if (currentIndex === randQustions.length - 1) {
         endOfQuiz()
      }
      else if (endQuiz == false) {
         setCurrentIndex(prev => prev + 1);
      }
   }

   //function to pre-emptively end quiz
   const seeResults = () => {
      setEndQuiz(true)
      setAvg((score / currentIndex) * 100)
   }

   const endOfQuiz = () => {
      setEndQuiz(true)
      setCurrentIndex(prev => prev + 1)
      setAvg((score / currentIndex) * 100)
      setIndex(0)
   }

   //function to result all variables 
   const reset = () => {
      setEndQuiz(false);
      setCurrentIndex(0);
      setScore(0)
      setShowAns(false)
      setBlock(false);
   }
   if (endQuiz) {
      return (
         <>
            {avg >= 80 &&
               <ShowConfetti whenToShow={endQuiz} />
            }
            <Grid
               direction="column"
               justify="center"
               alignItems="center"
               spacing={20}
               container
            >
               <Grid item xs={12}>
               </Grid>
               <div className="spacer"></div>
               <Grid item xs={12}>

                  <div className="top" style={{ textAlign: 'center' }}>
                     <ShowMessage avg={avg} />
                     <p>you scored <strong>{score}</strong> out of <strong>{currentIndex}</strong> = <strong>{avg.toFixed(1)}%</strong></p>
                  </div>
               </Grid>
               <Grid item xs={12}>
                  <Button style={{ padding: "5rem" }}

                     onClick={reset} >Start over?</Button>
               </Grid>
            </Grid>
         </>
      )
   }
   var imageURL = staticURL + 'topic/' + randQustions[currentIndex].topic + '/' + randQustions[currentIndex].question

   if (gamemode == 'translation') {
      return (
         <>
            <Container maxWidth="xl" sx={{ marginTop: '20px' }}>

               <Grid className="answers-row middle" container spacing={3}>


                  <Grid item xs={12}>
                     <Container className="question">
                        <CardMedia
                           id="realimage"
                           component="img"
                           image={imageURL}
                           alt="placeholder"
                           sx={{
                              padding: "1em 1em 0 1em", objectFit: "contain", width: '100%', height: '25vh',
                              margin: "0 auto",
                           }}
                        />
                     </Container>
                  </Grid>
                  {randQustions[currentIndex].answers.map((answer, key) => (
                     <Grid item xs={6}>
                        <Button style={{ padding: "10%" }}
                           className={showAns && answer.isCorrect ? 'ans' : ''}
                           onClick={(e) => { handleAnswerClick(answer.isCorrect, e) }}
                           key={key}
                           disabled={showAns}
                        >
                           <span style={{ height: "100%", fontFamily: 'Righteous, cursive', color: '#5f27cd', fontSize: '36px' }}> {answer.answerText}</span>
                        </Button>
                     </Grid>
                  ))}
                  <Grid item xs={12}>
                     <Button style={{ padding: "3%", color:'#0f0f0f' }}
                        className='secondary'
                        onClick={seeResults}
                        disabled={showAns || currentIndex === 0}>
                        See Results
                     </Button>
                  </Grid>

               </Grid>

            </Container>
         </>
      )
   }
   else if (gamemode == 'recognition') {
      return (
         <>
            <Container maxWidth="xl" sx={{ marginTop: '20px' }}>

               <Grid className="answers-row middle" container spacing={3}>

                  <Grid item xs={12}>

                     <Card style={{backgroundColor:'#bbaae5'}}>
                        <Box display="flex" alignItems="center" p={3} >
                           <span  style={{ height: "100%", fontFamily: 'Righteous, cursive', color: '#5f27cd', fontSize: '36px' }}>Question: What is {randQustions[currentIndex].question.slice(0, -4)}</span>
                        </Box>
                     </Card>
                  </Grid>

                  {randQustions[currentIndex].answers.map((answer, key) => (
                     <Grid item xs={4}>
                        <Button
                           className={showAns && answer.isCorrect ? 'ans' : ''}
                           onClick={(e) => { handleAnswerClick(answer.isCorrect, e) }}
                           key={key}
                           disabled={showAns}>
                           <CardMedia
                              id="realimage"
                              component="img"
                              image={staticURL + 'topic/' + randQustions[currentIndex].topic + '/' + answer.answerText + '.' + randQustions[0].question.split('.')[1]}
                              alt="placeholder"
                              sx={{
                                 padding: "1em 1em 0 1em", objectFit: "contain", width: '100%', height: '50.7vh',
                                 margin: "0 auto",
                              }}
                           />
                        </Button>
                     </Grid>

                  ))}
                  <Grid item xs={12}>
                     <Button style={{ padding: "3%", color:'#0f0f0f' }}
                        className='secondary'
                        onClick={seeResults}
                        disabled={showAns || currentIndex === 0}>
                        See Results
                     </Button>
                  </Grid>

               </Grid>

            </Container>
         </>
      )
   }
}

