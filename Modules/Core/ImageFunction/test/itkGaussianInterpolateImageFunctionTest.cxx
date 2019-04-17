/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include <iostream>

#include "itkImage.h"
#include "itkGaussianInterpolateImageFunction.h"
#include "itkMath.h"
#include "itkTestingMacros.h"

int itkGaussianInterpolateImageFunctionTest( int, char*[] )
{
  using ImageType = itk::Image< float, 2 >;
  using InterpolatorType = itk::GaussianInterpolateImageFunction< ImageType, float >;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  EXERCISE_BASIC_OBJECT_METHODS( interpolator, GaussianInterpolateImageFunction, InterpolateImageFunction );

  InterpolatorType::ArrayType sigma;
  sigma.Fill(1.0);
  interpolator->SetSigma(sigma);
  TEST_SET_GET_VALUE( sigma, interpolator->GetSigma() );

  InterpolatorType::RealType alpha = 1.0;
  interpolator->SetAlpha(alpha);
  TEST_SET_GET_VALUE( alpha, interpolator->GetAlpha() );

  ImageType::Pointer image = ImageType::New();

  ImageType::IndexType start;
  start.Fill(0);

  ImageType::SizeType size;
  size.Fill(3);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  ImageType::PointType origin;
  ImageType::SpacingType spacing;

  origin.Fill(0.0);
  spacing.Fill(1.0);

  image->SetOrigin(origin);
  image->SetSpacing(spacing);

  for ( unsigned int i = 0; i < 3; ++i )
  {
    for ( unsigned int j = 0; j < 3; ++j )
    {
      image->SetPixel({{i, j}}, i + j);
    }
  }

  interpolator->SetInputImage(image);

  InterpolatorType::OutputType expectedValues[5][5] = {{0.773964, 0.886982, 1.38698, 1.88698, 2.0},
    {0.886982, 1.0, 1.5, 2.0, 2.11302},
    {1.38698, 1.5, 2.0, 2.5, 2.61302},
    {1.88698, 2.0, 2.5, 3.0, 3.11302},
    {2.0, 2.11302, 2.61302, 3.11302, 3.22604}};

  ImageType::PointType point;
  point[0] = 0.0;

  for ( unsigned int i = 0; i < 5; ++i )
  {
    point[1] = 0.0;

    for ( unsigned int j = 0; j < 5; ++j )
    {
      InterpolatorType::OutputType computedValue = interpolator->Evaluate(point);

      if ( ! itk::Math::FloatAlmostEqual(computedValue, expectedValues[i][j], 7, 5e-6) )
      {
        std::cerr << "Error: computed and expected values are different" << std::endl;
        std::cerr << "Point: " << point << std::endl;
        std::cerr << "Computed: " << computedValue << std::endl;
        std::cerr << "Expectd: " << expectedValues[i][j] << std::endl;
        return EXIT_FAILURE;
      }

      point[1] += 0.5;
    }

    point[0] += 0.5;
  }

  return EXIT_SUCCESS;
}
